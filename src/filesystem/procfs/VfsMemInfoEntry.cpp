/**
 *   @file: VfsMemInfoEntry.cpp
 *
 *   @date: Oct 25, 2017
 * @author: Mateusz Midor
 */

#include <errno.h>
#include "VfsMemInfoEntry.h"
#include "MemoryManager.h"
#include "FrameAllocator.h"

using namespace memory;
namespace filesystem {

/**
 * @brief   The size of memory info is not known until the info string is built
 */
u32 VfsMemInfoEntry::get_size() const {
    return 0;
}

/**
 * @brief   Read the last "count" of kernel log bytes and clear the log
 * @return  Num of read bytes
 */
s64 VfsMemInfoEntry::read(void* data, u32 count) {
    // every second read is reading done
    if (reading_done) {
        reading_done = false;
        return 0;
    }

    if (count == 0)
        return 0;


    const kstd::string info = get_info();
    if (info.empty())
        return 0;

    u32 read_start = kstd::max((s64)info.length() - count, 0);
    u32 num_bytes_to_read = kstd::min(count, info.length());

    memcpy(data, info.c_str() + read_start, num_bytes_to_read);

    reading_done = true;
    return num_bytes_to_read;
}

kstd::string VfsMemInfoEntry::get_info() const {
//    MemoryManager& mm = MemoryManager::instance();
//    size_t free_memory = mm.get_total_memory_in_bytes() - mm.get_free_memory_in_bytes();
//    size_t total_memory = mm.get_total_memory_in_bytes();
//    env->printer->format("Used memory so far: % KB, total available: % MB\n", free_memory / 1024, total_memory / 1024 / 1024);

    size_t used_frames = FrameAllocator::get_used_frames_count();
    size_t total_frames = FrameAllocator::get_total_frames_count();
    return kstd::format("Used frames so far: %, total available: %\n", used_frames, total_frames);
}

s64 VfsMemInfoEntry::write(const void* data, u32 count) {
    return -EPERM;
}

} /* namespace filesystem */
