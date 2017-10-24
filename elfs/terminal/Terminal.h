/**
 *   @file: Terminal.h
 *
 *   @date: Jul 27, 2017
 * @author: Mateusz Midor
 */

#ifndef SRC_UTILS_TERMINAL_TERMINAL_H_
#define SRC_UTILS_TERMINAL_TERMINAL_H_

#include "KeyboardScanCodeSet.h"
#include "TerminalEnv.h"
#include "CommandHistory.h"
#include "CommandCollection.h"
#include "CommandLineInput.h"
#include "ustd.h"

namespace terminal {

class Terminal {
public:
    Terminal(u64 arg);
    void run();

private:
    bool init();
    void process_key(middlespace::Key key);
    ustd::string get_line();
    middlespace::Key get_key();
    void process_cmd(const ustd::string& cmd);

    template <class T>
    void install_cmd(const ustd::string& cmd) {
        cmds::CmdBase* task = new T((u64)&env);
        cmd_collection.install(cmd, task);
        user_input.install(cmd);
    }

    middlespace::Key    last_key = middlespace::Key::INVALID;
    CommandHistory      cmd_history;
    CommandCollection   cmd_collection;
    CommandLineInput    user_input;

    ScrollableScreenPrinter* printer;
    TerminalEnv env;

    // file descriptors
    int keyboard    = -1;   // for reading keyboard input
    int stdout      = -1;   // for reading stdout of child tasks(elf64)
};

} /* namespace terminal */

#endif /* SRC_UTILS_TERMINAL_TERMINAL_H_ */
