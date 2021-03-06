/**
 *   @file: Vga.h
 *
 *   @date: Oct 20, 2017
 * @author: Mateusz Midor
 */

#ifndef SRC_MIDDLESPACE_VGA_H_
#define SRC_MIDDLESPACE_VGA_H_

#include "types.h"

namespace middlespace {

/**
 * EGA 16 color pallete index, mode that GRUB lefts us in
 */
enum EgaColor : u8 {
    Black      = 0,
    Blue       = 1,
    Green      = 2,
    Cyan       = 3,
    Red        = 4,
    Magenta    = 5,
    Brown      = 6,
    LightGray  = 7,
    DarkGray   = 8,
    LightBlue  = 9,
    LightGreen = 10,
    LightCyan  = 11,
    LightRed   = 12,
    Pink       = 13,
    Yellow     = 14,
    White      = 15
};

/**
 * @see     https://en.wikipedia.org/wiki/Enhanced_Graphics_Adapter#/media/File:EGA_Table.PNG
 */
enum class EgaColor64 : u8 {
    Black           = 0,
    Gray            = 7,
    White           = 63,

    DarkRed         = 12,
    NormalRed       = 36,
    LightRed        = 39,

    DarkGreen       = 16,
    NormalGreen     = 2,
    LightGreen      = 18,

    DarkBlue        = 1,
    NormalBlue      = 25,
    LightBlue       = 11,

    DarkYellow      = 6,
    NormalYellow    = 62,
    LightYellow     = 55,

    NormalPink      = 45,
    LightPink       = 47,

    NormalViolet    = 13,
    NormalOrange    = 38,
};

/**
 * This is the structure describing single character in VGA buffer
 */
struct VgaCharacter {
    VgaCharacter(u16 c = 0) {
        character = c & 0xFF;
        fg_color = (c >> 8) & 0xF;
        bg_color = (c >> 12) & 0xF;
    }

    VgaCharacter(u8 c, u8 fg, u8 bg) {
        character = c;
        fg_color = fg;
        bg_color = bg;
    }

    operator u16() {
        return *((u16*)this);
    }

    s8      character;
    u8      fg_color    : 4;
    u8      bg_color    : 4;
};

}; // namespace middlespace

#endif /* SRC_MIDDLESPACE_VGA_H_ */
