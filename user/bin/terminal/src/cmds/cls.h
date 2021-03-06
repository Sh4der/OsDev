/**
 *   @file: cls.h
 *
 *   @date: Jan 9, 2018
 * @author: Mateusz Midor
 */

#ifndef USER_BIN_TERMINAL_SRC_CMDS_CLS_H_
#define USER_BIN_TERMINAL_SRC_CMDS_CLS_H_

#include "CmdBase.h"
#include "Monitor.h"
#include "ScrollableScreenPrinter.h"

namespace cmds {

class cls: public CmdBase {
public:
    cls(cstd::ustd::Monitor<terminal::ScrollableScreenPrinter>& printer) : printer(printer) {}
    u32 run(const CmdArgs& args);

private:
    cstd::ustd::Monitor<terminal::ScrollableScreenPrinter>&    printer;
};

} /* namespace cmds */

#endif /* USER_BIN_TERMINAL_SRC_CMDS_CLS_H_ */
