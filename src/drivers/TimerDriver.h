/**
 *   @file: TimerDriver.h
 *
 *   @date: Jun 27, 2017
 * @author: Mateusz Midor
 */

#ifndef SRC_DRIVERS_TIMERDRIVER_H_
#define SRC_DRIVERS_TIMERDRIVER_H_

#include <functional>
#include "DeviceDriver.h"

namespace drivers {

using TimerEvent = std::function<cpu::CpuState*(cpu::CpuState* cpu_state)>;

/**
 * @brief   This is a driver for system timer interrupt that is normally used for scheduling processes
 */
class TimerDriver: public DeviceDriver {
public:
    static s16 handled_interrupt_no();
    cpu::CpuState* on_interrupt(cpu::CpuState* cpu_state) override;
    void set_on_tick(const TimerEvent &event);
    void set_hz(u16 hz);

private:
    TimerEvent on_tick = [](cpu::CpuState* cpu_state) { return cpu_state; };
    Port8bit pit_channel_0  {0x40};
    Port8bit pit_channel_1  {0x41};
    Port8bit pit_channel_2  {0x42};
    Port8bit pit_cmd        {0x43};
};

} /* namespace drivers */

#endif /* SRC_DRIVERS_TIMERDRIVER_H_ */
