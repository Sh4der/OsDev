/**
 *   @file: PitDriver.cpp
 *
 *   @date: Jun 27, 2017
 * @author: Mateusz Midor
 */

#include "PitDriver.h"

using namespace cpu;

namespace drivers {

s16 PitDriver::handled_interrupt_no() {
    return Interrupts::Timer;
}

cpu::CpuState* PitDriver::on_interrupt(cpu::CpuState* cpu_state) {
    return on_tick(cpu_state);
}

void PitDriver::set_on_tick(const TimerEvent &event) {
    on_tick = event;
}

/**
 * @brief   Set PIT interrupt frequency
 * @param   hz Frequency in range 18 - 1'193'182 [Hz]
 * @ref     http://wiki.osdev.org/Programmable_Interval_Timer#Channel_0
 *          http://kernelx.weebly.com/programmable-interval-timer.html
 */
void PitDriver::set_hz(u16 hz) {
    const u8 MODE       = 0x36;         // channel 0 + lobyte/hibyte access + square wave generator + 16-bit binary mode
    const u32 DIVISOR   = 1193180 / hz;

    this->hz = hz;
    pit_cmd.write(MODE);
    pit_channel_0.write(DIVISOR & 0xFF);
    pit_channel_0.write(DIVISOR >> 8);
}

/**
 * @brief   Get PIT interrupt frequency
 */
u16 PitDriver::get_hz() const {
    return hz;
}

} /* namespace drivers */