/**
 *   @file: Elf64.cpp
 *
 *   @date: Sep 7, 2017
 * @author: Mateusz Midor
 */

#include "kstd.h"
#include "Elf64.h"
#include "StringUtils.h"

using namespace cstd;

namespace utils {

string Elf64::to_string(void* elf64_data) {
    Elf64_Ehdr* hdr = (Elf64_Ehdr*) elf64_data;
    string result;

    result += StringUtils::format("ELF header size: %\n", hdr->e_ehsize);
    string MAGIC { (s8)hdr->e_ident[Elf64_Ident::EI_MAG1], (s8)hdr->e_ident[Elf64_Ident::EI_MAG2], (s8)hdr->e_ident[Elf64_Ident::EI_MAG3]};
    result += MAGIC  + "\n";
    result += StringUtils::enum_to_str(hdr->e_ident[Elf64_Ident::EI_CLASS], "32bit=0x1", "64bit=0x2") + "\n";
    result += StringUtils::enum_to_str(hdr->e_ident[Elf64_Ident::EI_DATA], "Little endian=0x1", "Big endian=0x2") + "\n";
    result += StringUtils::enum_to_str(hdr->e_ident[Elf64_Ident::EI_VERSION], "Version 1=0x1") + "\n";
    result += StringUtils::enum_to_str(hdr->e_ident[Elf64_Ident::EI_OSABI], "SYSV=0x0", "HPUX=0x1", "STANDALONE=0xFF") + "\n";
    result += StringUtils::enum_to_str(hdr->e_ident[Elf64_Ident::EI_ABIVERSION], "SYSVR3=0x0") + "\n";
    result += StringUtils::enum_to_str(hdr->e_type, "No file type=0x0", "Relocatable=0x1", "Executable=0x2", "Shared object=0x3", "Core file=0x4") + "\n";
    result += StringUtils::format("Entry point at virt address: %\n", hdr->e_entry);
    result += StringUtils::format("Program header table file offset: %\n", hdr->e_phoff);
    result += StringUtils::format("Program header table entry size: %\n", hdr->e_phentsize);
    result += StringUtils::format("Program header table entry count: %\n", hdr->e_phnum);
    result += StringUtils::format("Section header table file offset: %\n", hdr->e_shoff);
    result += StringUtils::format("Section header table entry size: %\n", hdr->e_shentsize);
    result += StringUtils::format("Section header table entry count: %\n", hdr->e_shnum);
    result += StringUtils::format("String table section index: %\n", hdr->e_shstrndx);

    result += StringUtils::format("elf sections: \n");
    Elf64_Shdr* sections_start = (Elf64_Shdr*)((char*)elf64_data + hdr->e_shoff);
    Elf64_Shdr* section = sections_start;
    const char* section_names = (char*) ((char*)elf64_data + (sections_start + hdr->e_shstrndx)->sh_offset );
    for (auto i = 0; i < hdr->e_shnum; i++) {
        string esh_str = section_header_to_string(section_names, section);
        result += StringUtils::format("  %. %\n", i, esh_str);
        section++;
    }

    result += StringUtils::format("elf segments: \n");
    Elf64_Phdr* segment = (Elf64_Phdr*)((char*)elf64_data + hdr->e_phoff);
    for (auto i = 0; i < hdr->e_phnum; i++) {
        string esh_str = segment_header_to_string(segment);
        result += StringUtils::format("  %. %\n", i, esh_str);
        segment++;
    }

    return result;
}

string Elf64::section_header_to_string(const char* section_names, Elf64_Shdr* esh) {
    string type = StringUtils::enum_to_str(esh->sh_type,
           "NULL=0x0",
           "PROGBITS=0x1",
           "SYMTAB=0x2",
           "STRTAB=0x3",
           "RELA=0x4",
           "HASH=0x5",
           "DYNAMIC=0x6",
           "NOTE=0x7",
           "NO BITS=0x8",
           "REL=0x9",
           "SHLIB=0x0A",
           "DYNSYM=0x0B",
           "CONSTR.=0x0E",
           "DESTR.=0x0F",
           "PRECONSTRUC=0x10",
           "GROUP=0x11",
           "SYMTAB_SHNDX=0x12",
           "NUM=0x13",
           "OS_SPECYFIC=0x60000000"
    );

   string flags = StringUtils::flags_to_str(esh->sh_flags,
           "WRITE=0x1",
           "ALLOC=0x2",
           "EXEC=0x4",
           "MERGE=0x10",
           "STRINGS=0x20",
           "INFO_LINK=0x40",
           "LINK_ORDER=0x80",
           "OS_NONCONFORMING=0x100",
           "GROUP=0x200",
           "TLS=0x400",
           "MASKOS=0x0FF00000",
           "MASKPROC=0xF0000000",
           "ORDERED=0x4000000",
           "EXCLUDE=0x8000000"
   );


    u16 name_offset = esh->sh_name;
    const char* section_name = section_names + name_offset;
    return StringUtils::format("vaddr: %, vsize: %, name: %, type: %, flags: %", esh->sh_addr, esh->sh_size, section_name, type.c_str(), flags.c_str());
}

string Elf64::segment_header_to_string(Elf64_Phdr* esh) {
    string type =StringUtils::enum_to_str(esh->p_type,
           "NULL=0x0",
           "PT_LOAD=0x1",
           "PT_DYNAMIC=0x2",
           "PT_INTERP=0x3",
           "PT_NOTE=0x4",
           "PT_SHLIB=0x5",
           "PT_PHDR=0x6"
    );

    string flags =  StringUtils::flags_to_str(esh->p_flags,
            "X=0x1",
            "W=0x2",
            "R=0x4"
    );

    return StringUtils::format("vaddr: %, vsize: %, faddr: %, fsize: %, type: %, flags: %", esh->p_vaddr, esh->p_memsz, esh->p_offset, esh->p_filesz, type, flags);
}

/**
 * @brief   Check if given data is an ELF64
 */
bool Elf64::is_elf64(const void* elf64_data) {
    Elf64_Ehdr* hdr = (Elf64_Ehdr*) elf64_data;
    const unsigned char* ident = hdr->e_ident;

    if (ident[EI_MAG0] != '\x7f' || ident[EI_MAG1] != 'E' || ident[EI_MAG2] != 'L' || ident[EI_MAG3] != 'F')
        return false;   // not  ELF file

    if (ident[EI_CLASS] != 2)
        return false;   // not 64 bit

    return true;
}

/**
 * @brief   Load ELF64 executable from memory pointed by "elf64_data" into current address space
 * @return  Address of the executable entry point
 * @note    This can only work with statically linked -nostdlib binaries; very early stage of ELF64 support :)
 */
u64 Elf64::load_into_current_addressspace(void* elf64_data) {
    if (!is_elf64(elf64_data))
        return -1;

    Elf64_Ehdr* hdr = (Elf64_Ehdr*) elf64_data;

    Elf64_Phdr* segment = (Elf64_Phdr*)((char*)elf64_data + hdr->e_phoff);
    for (auto i = 0; i < hdr->e_phnum; i++) {
        if (segment->p_type == 1) { // PT_LOAD
            // TODO: add checks to ensure that segment->p_vaddr..segment->p_vaddr+segment->p_filesz stays out of kernel address space
            memset((u8*)segment->p_vaddr, 0, segment->p_memsz); // clear memory in case file size < memory size
            memcpy((u8*)segment->p_vaddr, (u8*)elf64_data + segment->p_offset, segment->p_filesz); // copy segment
        }

        segment++;
    }

    return hdr->e_entry;
}

/**
 * @brief   Get first unused virtual mem byte after loading elf with load_into_current_addressspace
 */
s64 Elf64::get_available_memory_first_byte(void* elf64_data) {
    u64 highest_byte = 0;
    Elf64_Ehdr* hdr = (Elf64_Ehdr*) elf64_data;

    Elf64_Phdr* segment = (Elf64_Phdr*)((char*)elf64_data + hdr->e_phoff);
    for (auto i = 0; i < hdr->e_phnum; i++) {
        if (segment->p_type == 1) { // PT_LOAD
            u64 segment_highest_bytes = segment->p_vaddr + segment->p_memsz;
            if (segment_highest_bytes > highest_byte)
                highest_byte = segment_highest_bytes;
        }

        segment++;
    }
    return highest_byte + 1;
}
} /* namespace utils */
