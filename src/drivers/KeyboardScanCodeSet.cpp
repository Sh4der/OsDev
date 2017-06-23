/**
 *   @file: ScanCodeSet1.cpp
 *
 *   @date: Jun 20, 2017
 * @author: Mateusz Midor
 */

#include "KeyboardScanCodeSet.h"

namespace drivers {


KeyboardScanCodeSet1::KeyboardScanCodeSet1 () {
    for (auto &e : key_ascii)
        e = "";

    key_ascii[0x02] = "1";
    key_ascii[0x03] = "2";
    key_ascii[0x04] = "3";
    key_ascii[0x05] = "4";
    key_ascii[0x06] = "5";
    key_ascii[0x07] = "6";
    key_ascii[0x08] = "7";
    key_ascii[0x09] = "8";
    key_ascii[0x0A] = "9";
    key_ascii[0x0B] = "0";
    key_ascii[0x0C] = "-";
    key_ascii[0x0D] = "=";
    key_ascii[0x0E] = "\x08";
    key_ascii[0x0F] = "\t";
    key_ascii[0x10] = "Q";
    key_ascii[0x11] = "W";
    key_ascii[0x12] = "E";
    key_ascii[0x13] = "R";
    key_ascii[0x14] = "T";
    key_ascii[0x15] = "Y";
    key_ascii[0x16] = "U";
    key_ascii[0x17] = "I";
    key_ascii[0x18] = "O";
    key_ascii[0x19] = "P";
    key_ascii[0x1A] = "[";
    key_ascii[0x1B] = "]";
    key_ascii[0x1C] = "\n";
    key_ascii[0x1E] = "A";
    key_ascii[0x1F] = "S";
    key_ascii[0x20] = "D";
    key_ascii[0x21] = "F";
    key_ascii[0x22] = "G";
    key_ascii[0x23] = "H";
    key_ascii[0x24] = "J";
    key_ascii[0x25] = "K";
    key_ascii[0x26] = "L";
    key_ascii[0x27] = " ;";
    key_ascii[0x28] = "'";
    key_ascii[0x29] = "`";
    key_ascii[0x2B] = "\\";
    key_ascii[0x2C] = "Z";
    key_ascii[0x2D] = "X";
    key_ascii[0x2E] = "C";
    key_ascii[0x2F] = "V";
    key_ascii[0x30] = "B";
    key_ascii[0x31] = "N";
    key_ascii[0x32] = "M";
    key_ascii[0x33] = ",";
    key_ascii[0x34] = ".";
    key_ascii[0x35] = "/";
    key_ascii[0x37] = "*";
    key_ascii[0x39] = " ";
    key_ascii[0x3B] = "F1";
    key_ascii[0x3C] = "F2";
    key_ascii[0x3D] = "F3";
    key_ascii[0x3E] = "F4";
    key_ascii[0x3F] = "F5";
    key_ascii[0x40] = "F6";
    key_ascii[0x41] = "F7";
    key_ascii[0x42] = "F8";
    key_ascii[0x43] = "F9";
    key_ascii[0x44] = "F10";
    key_ascii[0x47] = "7";
    key_ascii[0x48] = "8";
    key_ascii[0x49] = "9";
    key_ascii[0x4A] = "-";
    key_ascii[0x4B] = "4";
    key_ascii[0x4C] = "5";
    key_ascii[0x4D] = "6";
    key_ascii[0x4E] = "+";
    key_ascii[0x4F] = "1";
    key_ascii[0x50] = "2";
    key_ascii[0x51] = "3";
    key_ascii[0x52] = "0";
    key_ascii[0x53] = ".";

    /*
    key_description[0x01] = "escape pressed";
    key_description[0x02] = "1 pressed";
    key_description[0x03] = "2 pressed";
    key_description[0x04] = "3 pressed";
    key_description[0x05] = "4 pressed";
    key_description[0x06] = "5 pressed";
    key_description[0x07] = "6 pressed";
    key_description[0x08] = "7 pressed";
    key_description[0x09] = "8 pressed";
    key_description[0x0A] = "9 pressed";
    key_description[0x0B] = "0 (zero) pressed";
    key_description[0x0C] = "- pressed";
    key_description[0x0D] = "= pressed";
    key_description[0x0E] = "backspace pressed";
    key_description[0x0F] = "tab pressed";
    key_description[0x10] = "Q pressed";
    key_description[0x11] = "W pressed";
    key_description[0x12] = "E pressed";
    key_description[0x13] = "R pressed";
    key_description[0x14] = "T pressed";
    key_description[0x15] = "Y pressed";
    key_description[0x16] = "U pressed";
    key_description[0x17] = "I pressed";
    key_description[0x18] = "O pressed";
    key_description[0x19] = "P pressed";
    key_description[0x1A] = "[ pressed";
    key_description[0x1B] = "] pressed";
    key_description[0x1C] = "enter pressed";
    key_description[0x1D] = "left control pressed";
    key_description[0x1E] = "A pressed";
    key_description[0x1F] = "S pressed";
    key_description[0x20] = "D pressed";
    key_description[0x21] = "F pressed";
    key_description[0x22] = "G pressed";
    key_description[0x23] = "H pressed";
    key_description[0x24] = "J pressed";
    key_description[0x25] = "K pressed";
    key_description[0x26] = "L pressed";
    key_description[0x27] = " ; pressed";
    key_description[0x28] = "' (single quote) pressed";
    key_description[0x29] = "` (back tick) pressed";
    key_description[0x2A] = "left shift pressed";
    key_description[0x2B] = "\\ pressed";
    key_description[0x2C] = "Z pressed";
    key_description[0x2D] = "X pressed";
    key_description[0x2E] = "C pressed";
    key_description[0x2F] = "V pressed";
    key_description[0x30] = "B pressed";
    key_description[0x31] = "N pressed";
    key_description[0x32] = "M pressed";
    key_description[0x33] = ", pressed";
    key_description[0x34] = ". pressed";
    key_description[0x35] = "/ pressed";
    key_description[0x36] = "right shift pressed";
    key_description[0x37] = "(key_descriptionpad) * pressed";
    key_description[0x38] = "left alt pressed";
    key_description[0x39] = "space pressed";
    key_description[0x3A] = "CapsLock pressed";
    key_description[0x3B] = "F1 pressed";
    key_description[0x3C] = "F2 pressed";
    key_description[0x3D] = "F3 pressed";
    key_description[0x3E] = "F4 pressed";
    key_description[0x3F] = "F5 pressed";
    key_description[0x40] = "F6 pressed";
    key_description[0x41] = "F7 pressed";
    key_description[0x42] = "F8 pressed";
    key_description[0x43] = "F9 pressed";
    key_description[0x44] = "F10 pressed";
    key_description[0x45] = "NumberLock pressed";
    key_description[0x46] = "ScrollLock pressed";
    key_description[0x47] = "(key_descriptionpad) 7 pressed";
    key_description[0x48] = "(key_descriptionpad) 8 pressed";
    key_description[0x49] = "(key_descriptionpad) 9 pressed";
    key_description[0x4A] = "(key_descriptionpad) - pressed";
    key_description[0x4B] = "(key_descriptionpad) 4 pressed";
    key_description[0x4C] = "(key_descriptionpad) 5 pressed";
    key_description[0x4D] = "(key_descriptionpad) 6 pressed";
    key_description[0x4E] = "(key_descriptionpad) + pressed";
    key_description[0x4F] = "(key_descriptionpad) 1 pressed";
    key_description[0x50] = "(key_descriptionpad) 2 pressed";
    key_description[0x51] = "(key_descriptionpad) 3 pressed";
    key_description[0x52] = "(key_descriptionpad) 0 pressed";
    key_description[0x53] = "(key_descriptionpad) . pressed";
    key_description[0x57] = "F11 pressed";
    key_description[0x58] = "F12 pressed";
    key_description[0x81] = "escape released";
    key_description[0x82] = "1 released";
    key_description[0x83] = "2 released";
    key_description[0x84] = "3 released";
    key_description[0x85] = "4 released";
    key_description[0x86] = "5 released";
    key_description[0x87] = "6 released";
    key_description[0x88] = "7 released";
    key_description[0x89] = "8 released";
    key_description[0x8A] = "9 released";
    key_description[0x8B] = "0 (zero) released";
    key_description[0x8C] = "- released";
    key_description[0x8D] = "= released";
    key_description[0x8E] = "backspace released";
    key_description[0x8F] = "tab released";
    key_description[0x90] = "Q released";
    key_description[0x91] = "W released";
    key_description[0x92] = "E released";
    key_description[0x93] = "R released";
    key_description[0x94] = "T released";
    key_description[0x95] = "Y released";
    key_description[0x96] = "U released";
    key_description[0x97] = "I released";
    key_description[0x98] = "O released";
    key_description[0x99] = "P released";
    key_description[0x9A] = "[ released";
    key_description[0x9B] = "] released";
    key_description[0x9C] = "enter released";
    key_description[0x9D] = "left control released";
    key_description[0x9E] = "A released";
    key_description[0x9F] = "S released";
    key_description[0xA0] = "D released";
    key_description[0xA1] = "F released";
    key_description[0xA2] = "G released";
    key_description[0xA3] = "H released";
    key_description[0xA4] = "J released";
    key_description[0xA5] = "K released";
    key_description[0xA6] = "L released";
    key_description[0xA7] = "; released";
    key_description[0xA8] = "' (single quote) released";
    key_description[0xA9] = "` (back tick) released";
    key_description[0xAA] = "left shift released";
    key_description[0xAB] = "\\ released";
    key_description[0xAC] = "Z released";
    key_description[0xAD] = "X released";
    key_description[0xAE] = "C released";
    key_description[0xAF] = "V released";
    key_description[0xB0] = "B released";
    key_description[0xB1] = "N released";
    key_description[0xB2] = "M released";
    key_description[0xB3] = ", released";
    key_description[0xB4] = ". released";
    key_description[0xB5] = "/ released";
    key_description[0xB6] = "right shift released";
    key_description[0xB7] = "(key_descriptionpad) * released";
    key_description[0xB8] = "left alt released";
    key_description[0xB9] = "space released";
    key_description[0xBA] = "CapsLock released";
    key_description[0xBB] = "F1 released";
    key_description[0xBC] = "F2 released";
    key_description[0xBD] = "F3 released";
    key_description[0xBE] = "F4 released";
    key_description[0xBF] = "F5 released";
    key_description[0xC0] = "F6 released";
    key_description[0xC1] = "F7 released";
    key_description[0xC2] = "F8 released";
    key_description[0xC3] = "F9 released";
    key_description[0xC4] = "F10 released";
    key_description[0xC5] = "NumberLock released";
    key_description[0xC6] = "ScrollLock released";
    key_description[0xC7] = "(key_descriptionpad) 7 released";
    key_description[0xC8] = "(key_descriptionpad) 8 released";
    key_description[0xC9] = "(key_descriptionpad) 9 released";
    key_description[0xCA] = "(key_descriptionpad) - released";
    key_description[0xCB] = "(key_descriptionpad) 4 released";
    key_description[0xCC] = "(key_descriptionpad) 5 released";
    key_description[0xCD] = "(key_descriptionpad) 6 released";
    key_description[0xCE] = "(key_descriptionpad) + released";
    key_description[0xCF] = "(key_descriptionpad) 1 released";
    key_description[0xD0] = "(key_descriptionpad) 2 released";
    key_description[0xD1] = "(key_descriptionpad) 3 released";
    key_description[0xD2] = "(key_descriptionpad) 0 released";
    key_description[0xD3] = "(key_descriptionpad) . released";
    key_description[0xD7] = "F11 released";
    key_description[0xD8] = "F12 released";
    */
}

//kstd::string KeyboardScanCodeSet1::code_to_key_description(u8 key_code) const {
//    return key_description[key_code];
//}

const kstd::string& KeyboardScanCodeSet1::code_to_ascii(u8 key_code) const {
    return key_ascii[key_code];
}

}   /* namespace drivers */