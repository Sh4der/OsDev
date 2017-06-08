/**
 *   @file: CpuInfo.cpp
 *
 *   @date: Jun 7, 2017
 * @author: mateusz
 */

#include "CpuInfo.h"

CpuInfo::CpuInfo() {
    getCpuVendor(vendor);
}

void CpuInfo::print(ScreenPrinter &p) {
    p.format("CPU: %\n", vendor);
}

void CpuInfo::getCpuVendor(char buff[17]) {
    buff[16] = '\0';

    __asm__ (
        "movq %0, %%rdi\n\t"
        "xor %%rax, %%rax;\n\t"
         "cpuid;\n\t"
         "mov %%ebx, (%%rdi);\n\t"
         "mov %%edx, 4(%%rdi);\n\t"
         "mov %%ecx, 8(%%rdi);\n\t"
        : // no output used
        : "g" (vendor)
        : "memory", "%eax", "%ebx", "%ecx", "%edx", "%rdi"
    );
}