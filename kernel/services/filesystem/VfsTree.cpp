/**
 *   @file: VfsTree.cpp
 *
 *   @date: Jul 23, 2018
 * @author: Mateusz Midor
 */

#include "VfsTree.h"
#include "StringUtils.h"
#include "VfsCachedEntry.h"
#include "VfsRamDirectoryEntry.h"
#include "List.h"

using namespace cstd;
using namespace middlespace;

namespace filesystem {

/**
 * @brief   Cut off the last segment of "path" and return it, "path" itself becomes shorter
 * @note    If last segment cut, "path" becomes the root "/"
 */
static string snap_path_tail(string& path) {
    string result = StringUtils::snap_tail(path, '/');

    // when last segment has been cut, the root remains
    if (path.empty())
        path = "/";

    return result;
}

/**
 * @brief   Get entry in "parent_dir"
 * @param   name Entry name. Case sensitive
 * @return  Valid entry if exists, nullptr otherwise
 */
static VfsEntryPtr get_entry_in_dir(const string& name, VfsEntryPtr parent_dir) {
    VfsEntryPtr result;
    auto on_entry = [&](VfsEntryPtr e) -> bool {
        if (e->get_name() == name) {
            result = e;
            return false;   // entry found. stop enumeration
        }
        else
            return true;    // continue searching for entry
    };

    parent_dir->enumerate_entries(on_entry);
    return result;
}

/**
 * @brief   Prepare the vfs tree to work with dynamic memory once it is available in system
 */
void VfsTree::install() {
    auto root_dir = std::make_shared<VfsRamDirectoryEntry>("/"); // empty root dir
    entry_cache.install(root_dir);
}

/**
 * @brief   Attach "entry" under given "parent_path", return error code on error
 */
utils::SyscallResult<void> VfsTree::attach(const VfsEntryPtr& entry, const UnixPath& parent_path) {
    // get the parent from depths of virtual file system and cache it so we can attach to it
    auto fd = get_or_bring_entry_to_cache(parent_path);

    if (!fd) {
        klog.format("VfsTree::attach: can't attach to '%s'", parent_path);
        return {fd.ec};
    }

    if (!entry_cache[fd.value]->attach_entry(entry)) {
        klog.format("VfsTree::attach: entry '%s' already exists at '%s'", entry->get_name(), parent_path);
        return {ErrorCode::EC_EXIST};
    }

    return {ErrorCode::EC_OK};
}

/**
 * @brief   Open a file pointed by "path" and return its file descriptor on success, or error code otherwise
 */
utils::SyscallResult<GlobalFileDescriptor> VfsTree::open(const UnixPath& path) {
    auto fd = get_or_bring_entry_to_cache(path);

    if (!fd) {
        klog.format("VfsTree::open: can't open '%s'", path);
        return {fd.ec}; // propagate error code
    }

    // open increases refcount
    entry_cache[fd.value]->refcount++;
    return fd;
}

/**
 * @brief   Close the file and remove it from cache if no longer needed
 */
void VfsTree::close(GlobalFileDescriptor fd) {
    if (!entry_cache.is_in_cache(fd))
        return;

    entry_cache[fd]->refcount--;

    // remove from cache if no longer needed. This needs testing
    if (entry_cache[fd]->refcount == 0 && entry_cache[fd]->attachment_count() == 0)
        entry_cache.deallocate_entry(fd);
}

/**
 * @brief   Get the entry pointed by "path" from cache or bring it from virtual file system to cache
 *          and return its file descriptor or error code on error
 */
utils::SyscallResult<GlobalFileDescriptor> VfsTree::get_or_bring_entry_to_cache(const UnixPath& path) {
    // if file already in cache, just return the file descriptor
    if (auto fd = entry_cache.get_fd_for_path(path))
        return {fd.value};

    // otherwise lookup the file, allocate it in cache and return the file descriptor
    VfsEntryPtr e = lookup_entry(path);
    if (!e) {
        klog.format("VfsTree::get_or_bring_entry_to_cache: entry '%s' not exists", path);
        return {ErrorCode::EC_NOENT};
    }

    auto fd = entry_cache.allocate_entry(e, path);
    if (!fd) {
        klog.format("VfsTree::get_or_bring_entry_to_cache: open file limit reached");
        return {ErrorCode::EC_MFILE};
    }

    return {fd.value};
}

/**
 * @brief   Check for entry existence at "path"
 */
bool VfsTree::exists(const UnixPath& path) const {
    if (entry_cache.get_fd_for_path(path))
        return true;

    if (lookup_entry(path))
        return true;

    return false;
}

/**
 * @brief   Lookup entry pointed by "path" in the depths of virtual file system.
 *          Directly cached entries are not taken into account, but their children are,
 *          eg. for mountpoints that live in cache but their children are maintained by mountpoints themselves
 */
VfsEntryPtr VfsTree::lookup_entry(const UnixPath& path) const {
    List<string> path_segments;

    // first find the cached parent on the path. Root "/" can also be the parent
    string subpath = path;
    Optional<GlobalFileDescriptor> fd;

    // ascend up the path eventually to the root if no parent is found in cache
    while (!fd) {
        string segment = snap_path_tail(subpath);
        path_segments.push_front(segment);
        fd = entry_cache.get_fd_for_path(subpath);
    }

    // get the parent
    VfsEntryPtr e = entry_cache[fd.value];

    // now descent from the parent along the remembered segments
    for (const auto& path_segment : path_segments) {
        if (e->get_type() != VfsEntryType::DIRECTORY) {
            klog.format("VfsTree::lookup_entry: path segment '%' is not a directory\n", e->get_name());
            return {};   // path segment is not a directory. this is error
        }

        e = get_entry_in_dir(path_segment, e);
        if (!e) {
            klog.format("VfsTree::lookup_entry: path segment '%' does not exist\n", path_segment);
            return {};   // path segment does not exist. this is error
        }
    }

    return e;
}


} /* namespace filesystem */
