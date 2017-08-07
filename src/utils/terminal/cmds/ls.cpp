/**
 *   @file: ls.cpp
 *
 *   @date: Jul 27, 2017
 * @author: Mateusz Midor
 */

#include "ls.h"

using namespace kstd;
using namespace filesystem;
namespace cmds {

void ls::run() {
    if (env->volumes.empty())
        env->printer->format("No volumes installed\n");


    OnEntryFound on_entry = [&](const Fat32Entry& e) -> bool {
        if (e.is_directory)
            env->printer->format("[%]\n", e.name);
        else
            env->printer->format("% - %B\n", e.name, e.size);

        return true;
    };

    string path;
    if (env->cmd_args.size() == 2)
        path = env->cwd + "/" + env->cmd_args[1];
    else
        path = env->cwd;

    VolumeFat32* v = env->volume;
    Fat32Entry e;
    if (!v->get_entry(path, e)) {
        env->printer->format("ls: path '%' does not exist\n", path);
        return;
    }

    if (e.is_directory)
        v->enumerate_directory_entry(e, on_entry);
    else
        env->printer->format("% - %B\n", e.name, e.size);

}
} /* namespace cmds */
