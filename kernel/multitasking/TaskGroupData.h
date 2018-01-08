/**
 *   @file: TaskGroupData.h
 *
 *   @date: Nov 27, 2017
 * @author: Mateusz Midor
 */

#ifndef KERNEL_MULTITASKING_TASKGROUPDATA_H_
#define KERNEL_MULTITASKING_TASKGROUPDATA_H_

#include <array>
#include <memory>
#include "String.h"
#include "VfsEntry.h"

namespace multitasking {

/**
 * @brief   This class holds data common to a task group (or a PROCESS, if you wish to call it so)
 */
class TaskGroupData {
public:
    TaskGroupData(u64 pml4_phys_addr, const kstd::string& cwd, size_t heap_low_limit, size_t heap_high_limit);
    ~TaskGroupData();
    void* alloc_static(size_t size);

    size_t                                  heap_low_limit;     // last address allocated for the heap, current program break
    size_t                                  heap_high_limit;    // last address allocable for the heap
    kstd::string                            cwd;                // current working directory of task group
    u64                                     pml4_phys_addr;     // address space of task group. This uniquely identifies the group
    std::array<filesystem::VfsEntryPtr, 16> files;              // list of open files. TODO: concurrent access to the same file. How?

private:
    void close_files();
    void release_address_space();
};

using TaskGroupDataPtr = std::shared_ptr<TaskGroupData>;

} /* namespace multitasking */

#endif /* KERNEL_MULTITASKING_TASKGROUPDATA_H_ */
