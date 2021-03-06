/**
 *   @file: VfsRamFifoEntry.h
 *
 *   @date: Oct 17, 2017
 * @author: Mateusz Midor
 */

#ifndef SRC_FILESYSTEM_VFSRAMFIFOENTRY_H_
#define SRC_FILESYSTEM_VFSRAMFIFOENTRY_H_

#include "VfsEntry.h"
#include "TaskList.h"

namespace ipc {

/**
 * @brief   This class is VfsEntry implementation for an in-memory fifo (first in first out) file entry.
 * @note    It acts like FIFO; you always read the head of it, and write the tail, thus get_position() always returns 0.
 *          It blocks reader if empty, it blocks writer if full.
 */
class VfsRamFifoEntry: public filesystem::VfsEntry {
public:
    VfsRamFifoEntry(const cstd::string& name) : name(name) {}

    // [common interface]
    const cstd::string& get_name() const override                                               { return name; }
    filesystem::VfsEntryType get_type() const override                                                      { return filesystem::VfsEntryType::PIPE; }

    // [file interface]
    utils::SyscallResult<u64> get_size() const override                                         { return {size}; }
    utils::SyscallResult<u64> read(filesystem::EntryState* state, void* data, u32 count) override;
    utils::SyscallResult<u64> write(filesystem::EntryState* state, const void* data, u32 count) override;
    utils::SyscallResult<void> seek(filesystem::EntryState* state, u32 new_position) override               { return {INVALID_OP}; }
    utils::SyscallResult<void> truncate(filesystem::EntryState* state, u32 new_size) override;
    utils::SyscallResult<u64> get_position(filesystem::EntryState* state) const override                    { return {0}; }

private:
    static constexpr u32        BUFF_SIZE           {512};
    const cstd::string          name;
    u8                          buff[BUFF_SIZE];
    u32                         size                {0};
    multitasking::TaskList      read_wait_list;
    multitasking::TaskList      write_wait_list;

};

} /* namespace ipc */

#endif /* SRC_FILESYSTEM_VFSRAMFIFOENTRY_H_ */
