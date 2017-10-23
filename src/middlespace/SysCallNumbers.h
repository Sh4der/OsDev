/**
 *   @file: SyscallNumbers.h
 *
 *   @date: Oct 20, 2017
 * @author: Mateusz Midor
 */

#ifndef SRC_MIDDLESPACE_SYSCALLNUMBERS_H_
#define SRC_MIDDLESPACE_SYSCALLNUMBERS_H_

namespace middlespace {

/**
 * @brief   This enum defines system call numbers(asm: syscall) same as for Linux
 * @see     http://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/
 */
enum SysCallNumbers {
    // POSIX
    FILE_READ               = 0,
    FILE_WRITE              = 1,
    FILE_OPEN               = 2,
    FILE_CLOSE              = 3,
//    FILE_GETDENTS           = 0xdc, //220
    EXIT                    = 60,
    EXIT_GROUP              = 231,

    // NON-POSIX
    VGA_CURSOR_SETVISIBLE   = 500,
    VGA_CURSOR_SETPOS       = 501,
    VGA_SET_AT              = 502,
    FILE_ENUMERATE          = 600,
    ELF_RUN                 = 700,
};

} /* namespace middlespace */

#endif /* SRC_MIDDLESPACE_SYSCALLNUMBERS_H_ */