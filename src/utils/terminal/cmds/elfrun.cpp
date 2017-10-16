/**
 *   @file: elfrun.cpp
 *
 *   @date: Sep 12, 2017
 * @author: Mateusz Midor
 */

#include "kstd.h"
#include "ElfRunner.h"
#include "elfrun.h"

using namespace kstd;
using namespace utils;
using namespace filesystem;

namespace cmds {

/**
 * ELF image is organized as follows:
 * 0...x -> process image
 * ELF_VIRTUAL_MEM_BYTES - stack size -> process stack
 */
void elfrun::run() {
    if (env->volumes.empty()) {
        env->printer->format("elfrun: no volumes installed\n");
        return;
    }

    if (env->cmd_args.size() < 2) {
        env->printer->format("elfrun: please specify file name\n");
        return;
    }

    string name = env->cmd_args[1];
    string filename = env->cwd + "/" + name;
    VolumeFat32* v = env->volume;
    auto e = v->get_entry(filename);
    if (!e) {
        env->printer->format("elfrun: file % doesnt exist\n", filename);
        return;
    }

    if (e.is_directory()) {
        env->printer->format("elfrun: % is a directory\n", filename);
        return;
    }

    // read elf file data
    u32 size = e.get_size();
    u8* elf_data = new u8[size];
    e.read(elf_data, size);

    // run the elf
    userspace::ElfRunner runner;
    if (!runner.run(elf_data, vector<string>(env->cmd_args.cbegin() + 1, env->cmd_args.cend())))
        env->printer->format("elfrun: not enough memory to run elf\n");
    delete[] elf_data;
}

} /* namespace cmds */
