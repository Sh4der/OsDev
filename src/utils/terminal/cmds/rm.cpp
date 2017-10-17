/**
 *   @file: rm.cpp
 *
 *   @date: Jul 28, 2017
 * @author: Mateusz Midor
 */

#include "rm.h"
#include "VolumeFat32.h"
#include "kstd.h"
#include "MassStorageMsDos.h"
#include "DriverManager.h"

using namespace kstd;
using namespace utils;
using namespace drivers;
using namespace filesystem;
namespace cmds {

void rm::run() {
    if (env->cmd_args.size() < 2) {
        env->printer->format("rm: please specify file name\n");
        return;
    }

    string path = env->cmd_args[1];
    string absolute_path;

    if (env->cwd.back() == '/')
        absolute_path = format("%%", env->cwd, path);
    else
        absolute_path = format("%/%", env->cwd, path);

    if (!env->vfs_manager.delete_entry(absolute_path))
        env->printer->format("rm: unable to remove %\n", absolute_path);
}
} /* namespace cmds */
