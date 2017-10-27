/**
 *   @file: ls.cpp
 *
 *   @date: Oct 26, 2017
 * @author: Mateusz Midor
 */


#include "_start.h"
#include "ustd.h"
#include "utils.h"


const char ERROR_PATH_NOT_EXISTS[]  = "ls: path doesnt exist\n";
const char ERROR_OPENING_DIR[]      = "ls: cant open specified directory\n";
char buff[256];
char size_str[12];

using namespace ustd;
using namespace middlespace;

void print_file(const char name[], u32 size) {
    long_to_str(size, 10, size_str);
    print(name);
    print(" - ");
    print(size_str);
    print(" bytes \n");
}

void print_dir(const char name[]) {
    print("[");
    print(name);
    print("]\n");
}

/**
 * @brief   Entry point
 * @return  0 on success, 1 on error
 */
int main(int argc, char* argv[]) {
    char* path;
    if (argc == 1)
        path = syscalls::getcwd(buff, sizeof(buff) - 1);
    else
        path = argv[1];


    // check exists
    struct stat s;
    if (syscalls::stat(path, &s) < 0) {
        print(ERROR_PATH_NOT_EXISTS);
        return 1;
    }

    // ls file
    if (s.st_mode == S_IFREG) {
        print_file(path, s.st_size);
        return 0;
    }

    // ls directory
    int fd = syscalls::open(path);
    if (fd < 0) {
        print(ERROR_OPENING_DIR);
        return 1;
    }

    u32 MAX_ENTRIES = 128; // should there be more in a single dir?
    FsEntry* entries = new FsEntry[MAX_ENTRIES];

    int count = syscalls::enumerate(fd, entries, MAX_ENTRIES);
    print("\n");
    for (int i = 0; i < count; i++)
        if (entries[i].is_directory)
            print_dir(entries[i].name);
        else
            print_file(entries[i].name, entries[i].size);

    delete[] entries;
    syscalls::close(fd);

    return 0;
}