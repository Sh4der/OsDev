/**
 *   @file: CmdBase.h
 *
 *   @date: Jul 27, 2017
 * @author: Mateusz Midor
 */

#ifndef SRC_UTILS_TERMINAL_CMDS_CMDBASE_H_
#define SRC_UTILS_TERMINAL_CMDS_CMDBASE_H_

#include "TerminalEnv.h"

namespace cmds {

class CmdBase {
public:
    CmdBase(terminal::TerminalEnv* arg);
    virtual ~CmdBase() {}
    virtual void run() = 0;

protected:
    terminal::TerminalEnv* env;

};

} /* namespace cmds */

#endif /* SRC_UTILS_TERMINAL_CMDS_CMDBASE_H_ */