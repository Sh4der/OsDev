/**
 *   @file: rmdir.cpp
 *
 *   @date: Oct 27, 2017
 * @author: Mateusz Midor
 */

#include "_start.h"
#include "utils.h"

const char ERROR_NO_INPUT_FILE[] = "Please provide directory path";
const char ERROR_CANT_REMOVE[] = "Cant remove given directory\n";


/**
 * @brief   Entry point
 */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        print(ERROR_NO_INPUT_FILE);
        return 1;
    }

    const char* path = argv[1];

    int result = syscalls::rmdir(path);
    if (result < 0) {
        print(ERROR_CANT_REMOVE);
        return -1;
    }

    return 0;
}
