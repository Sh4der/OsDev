/**
 *   @file: PageTables.cpp
 *
 *   @date: Oct 5, 2017
 * @author: Mateusz Midor
 */

#include "kstd.h"
#include "PageTables.h"
#include "HigherHalf.h"

using namespace memory;
namespace hardware {

PageTables64  PageTables::kernel_page_tables  __attribute__ ((aligned (4096)));

/**
 * @brief   Map the kernel -2GB virtual memory address space at physical address 0 (where it already is loaded by bootloader)
 *          After this, the first 1GB identity mapping is no longer available and accessing any physical address will end up with page fault
 * @note    Right now only the single -2GB is mapped, -1GB stays unmapped as there is no need for so much virtual address space
 *          Page hierarchy is as follows:
 *          cr3 -> pml4
 *                  -pdpt
 *                     -pde
 *                       -pte (not used when 2MB HUGE pages are used)
 */
void PageTables::map_and_load_kernel_address_space_at_memory_start() {
    prepare_higher_half_kernel_page_tables(kernel_page_tables);
    u64 pml4_physical_address = HigherHalf::virt_to_phys(kernel_page_tables.pml4);
    load_address_space(pml4_physical_address);
}

/**
 * @brief   Map virtual address space 0..num_bytes-1 to physical phys_addr..phys_addr+num_bytes-1
 * @param   pml4_phys_addr Physical address of the already allocated PageTables64
 * @note    Temporary solution until proper frame allocator and memory manager is developed
 */
void PageTables::map_elf_address_space(size_t pml4_phys_addr) {
    // we need to use kernel virtual addresses for configuring the pages, since no lower 1gb identity mapping exists
    size_t page_tables_virt_addr = HigherHalf::phys_to_virt(pml4_phys_addr);

    // alloc page tables at 4k aligned virtual address
    PageTables64* pt = new ((void*)page_tables_virt_addr)PageTables64;

    // zero the page tables
    memset(pt, 0, sizeof(PageTables64));

    // map kernel virtual address space at -2GB
    prepare_higher_half_kernel_page_tables(*pt);

    // map user elf virtual address space at 1GB
    prepare_elf_page_tables(pt);

    size_t pml4_physical_address = HigherHalf::virt_to_phys(pt->pml4);
}

/**
 * @brief Fill PageTables64 with mapping of -2..-1GB virt addresses to 0..1GB phys addresses
 */
void PageTables::prepare_higher_half_kernel_page_tables(PageTables64& pt) {
    const u16 PRESENT_WRITABLE_USERSPACE = PageAttr::PRESENT | PageAttr::WRITABLE | PageAttr::USER_ACCESSIBLE;
    pt.pml4[511] = HigherHalf::virt_to_phys(pt.pdpt) | PRESENT_WRITABLE_USERSPACE;
    pt.pdpt[510] = HigherHalf::virt_to_phys(pt.pde_kernel) | PRESENT_WRITABLE_USERSPACE;
    for (u16 i = 0; i < 512; i++)
        pt.pde_kernel[i] = (0x200000 * i) | PRESENT_WRITABLE_USERSPACE | PageAttr::HUGE_PAGE;
}

/**
 * @brief   Fill PageTables64 pml4 and pdpt tables with mapping of lower 1GB virtual memory
 */
void PageTables::prepare_elf_page_tables(PageTables64* pt) {
    const u16 PRESENT_WRITABLE_USERSPACE = PageAttr::PRESENT | PageAttr::WRITABLE | PageAttr::USER_ACCESSIBLE;
    pt->pml4[0] = HigherHalf::virt_to_phys(pt->pdpt) | PRESENT_WRITABLE_USERSPACE;
    pt->pdpt[0] = HigherHalf::virt_to_phys(pt->pde_user) | PRESENT_WRITABLE_USERSPACE;
    // specific frame allocation in pde_user for the lower 1GB will happen in PageFault handler
}

/**
 * @brief   Get kernel page tables root physical address
 */
size_t PageTables::get_kernel_pml4_phys_addr() {
    return HigherHalf::virt_to_phys(kernel_page_tables.pml4);
}

/**
 * @brief   Load pml4_physical_address(page tables root) into cr3, effectively setting new memory address space
 */
void PageTables::load_address_space(size_t pml4_physical_address) {
    asm volatile (
            "mov %%rax, %%cr3       ;"
            :
            : "a"(pml4_physical_address)
            :
    );
}

/**
 * @brief   Get Kernel space virtual address of page in "pml4_phys_addr" address space that contains "virtual_address"
 */
u64* PageTables::get_page_for_virt_address(size_t virtual_address, size_t pml4_phys_addr) {
    u16 pml4_index = (virtual_address >> 39) & 511;
    u16 pdpt_index = (virtual_address >> 30) & 511;
    u16 pde_index = (virtual_address >> 21) & 511;

    u64* pml4_virt_addr = (u64*)HigherHalf::phys_to_virt(pml4_phys_addr);
    u64* pdpt_virt_addr = (u64*)HigherHalf::phys_to_virt(pml4_virt_addr[pml4_index] & ~4095); // & ~4095 to remove page flags
    u64* pde_virt_addr =  (u64*)HigherHalf::phys_to_virt(pdpt_virt_addr[pdpt_index] & ~4095); // & ~4095 to remove page flags
    return  &pde_virt_addr[pde_index];
}

} /* namespace hardware */
