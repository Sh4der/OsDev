/**
 *   @file: mb2.cpp
 *
 *   @date: Sep 4, 2017
 * @author: Mateusz Midor
 */

#include "mb2.h"
#include "Multiboot2.h"

using namespace utils;
namespace cmds {

void mb2::run() {
    env->printer->format(Multiboot2::to_string());
}

} /* namespace cmds */