/**
 *   @file: CommandCollection.h
 *
 *   @date: Jul 27, 2017
 * @author: Mateusz Midor
 */

#ifndef SRC_UTILS_TERMINAL_COMMANDCOLLECTION_H_
#define SRC_UTILS_TERMINAL_COMMANDCOLLECTION_H_

#include <tuple>
#include "kstd.h"
#include "Task.h"

namespace terminal {

struct Command {
    kstd::string            name;
    multitasking::TaskPtr   task;
};

class CommandCollection {
public:
    multitasking::TaskPtr get(const kstd::string& cmd_name);
    std::tuple<bool, kstd::string> filter(const kstd::string& pattern);
    void install(const kstd::string name, multitasking::TaskPtr task);

private:
    kstd::vector<Command> commands;
};

} /* namespace terminal */

#endif /* SRC_UTILS_TERMINAL_COMMANDCOLLECTION_H_ */