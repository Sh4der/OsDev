/**
 *   @file: mkdir.cpp
 *
 *   @date: Aug 1, 2017
 * @author: Mateusz Midor
 */

#include "mkdir.h"

using namespace ustd;
namespace cmds {

void mkdir::run() {
    if (env->cmd_args.size() < 2) {
        env->printer->format("mkdir: please specify dir name\n");
        return;
    }

    string dirname = make_absolute_filename(env->cmd_args[1]);

    if (!env->vfs_manager.create_entry(dirname, true)) {
        env->printer->format("mkdir: could not create directory '%'\n", dirname);
    }
}

} /* namespace cmds */