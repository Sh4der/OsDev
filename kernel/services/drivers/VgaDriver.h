/**
 *   @file: VgaDriver.h
 *
 *   @date: Jun 26, 2017
 * @author: Mateusz Midor
 */

#ifndef SRC_DRIVERS_VGADRIVER_H_
#define SRC_DRIVERS_VGADRIVER_H_

#include "DeviceDriver.h"
#include "Vga.h"
#include "Port.h"

namespace drivers {

class VgaDriver : public DeviceDriver {
public:
    static s16 handled_interrupt_no();
    hardware::CpuState* on_interrupt(hardware::CpuState* cpu_state) override;

    // common interface
    u16 screen_width() const;
    u16 screen_height() const;

    // text interface
    void set_text_mode_90_30();
    middlespace::VgaCharacter& char_at(u16 x, u16 y) const;
    void flush_char_buffer(const middlespace::VgaCharacter* buff);
    void set_cursor_visible(bool visible);
    void set_cursor_pos(u8 x, u8 y);
    void clear_screen(middlespace::EgaColor color = middlespace::EgaColor::Black);
    void print(u16 x, u16 y, const char* text, middlespace::EgaColor fg = middlespace::EgaColor::White, middlespace::EgaColor bg = middlespace::EgaColor::Black);

    // graphical interface
    void set_graphics_mode_320_200_256();
    void put_pixel(u16 x, u16 y, middlespace::EgaColor64 color_index) const;
    void flush_video_buffer(const middlespace::EgaColor64* buff);

private:
    void write_registers(u8* registers) const;
    u8* get_framebuffer_segment() const;

    u8* framebuffer_segment                             {nullptr};
    u8* framebuffer_segment_copy                        {nullptr};
    middlespace::VgaCharacter* const vga                {(middlespace::VgaCharacter*)0xFFFFFFFF800b8000};
    u16 width                                           {80};  // in characters (text mode)
    u16 height                                          {25};  // in characters (text mode)
    // text mode
    hardware::Port8bit cursor_cmd_port                  {0x3D4};
    hardware::Port8bit cursor_data_port                 {0x3D5};
    // graphics mode
    hardware::Port8bit misc_port                        {0x3C2};
    hardware::Port8bit crtc_index_port                  {0x3D4};
    hardware::Port8bit crtc_data_port                   {0x3D5};
    hardware::Port8bit sequence_index_port              {0x3C4};
    hardware::Port8bit sequence_data_port               {0x3C5};
    hardware::Port8bit graphics_controller_index_port   {0x3CE};
    hardware::Port8bit graphics_controller_data_port    {0x3CF};
    hardware::Port8bit attribute_controller_index_port  {0x3C0};
    hardware::Port8bit attribute_controller_read_port   {0x3C1};
    hardware::Port8bit attribute_controller_write_port  {0x3C0};
    hardware::Port8bit attribute_controller_reset_port  {0x3DA};
};

} /* namespace drivers */

#endif /* SRC_DRIVERS_VGADRIVER_H_ */
