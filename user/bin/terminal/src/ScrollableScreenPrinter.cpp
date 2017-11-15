/**
 *   @file: ScrollableScreenPrinter.cpp
 *
 *   @date: Jun 5, 2017
 * @author: Mateusz Midor
 */

#include "ScrollableScreenPrinter.h"
#include "syscalls.h"

using namespace drivers;
namespace terminal {

ScrollableScreenPrinter::ScrollableScreenPrinter(u16 left, u16 top, u16 right, u16 bottom) :
        left(left), top(top), right(right-1), bottom(bottom), // -1 for scrollbar
        cursor(left, top, right - 1, bottom) {

    syscalls::vga_get_width_height(&vga_width, &vga_height);
    vga_buffer = new VgaCharacter[vga_width * vga_height];
    printable_area_width = right - left + 1 - 1;  // +1 because if right=1 and left=0 it makes 2 columns, -1 for scroll bar
    printable_area_height = bottom - top + 1; // +1 because if bottom=1 and top=0 it makes 2 rows
}

ScrollableScreenPrinter::~ScrollableScreenPrinter() {
    delete[] vga_buffer;
}


void ScrollableScreenPrinter::putc(const char c) {
    if (c == '\n') {
        newline();
    } else if (c == '\t') {
        tab();
    } else if (c == '\x08') {
        backspace();
    } else {
       // set_at(cursor, VgaCharacter(c,  foreground, background)); // redraw will do it
        cursor++;
        lines.putc(c);
    }

    // enforce text buffer "word" wrap
    if (lines.back().length() == printable_area_width)
        lines.newline();

    // scroll if writing below bottom of the screen
    scroll_to_end();
}

void ScrollableScreenPrinter::newline() {
    cursor.newline();
    lines.newline();
    scroll_to_end();
}

void ScrollableScreenPrinter::tab() {
    putc(' ');
    putc(' ');
    lines.putc(' ');
    lines.putc(' ');
}

void ScrollableScreenPrinter::backspace() {
    // back cursor by 1 char
    cursor--;

    // clear character under cursor
    set_at(cursor, VgaCharacter { .character = ' ', .fg_color = foreground, .bg_color = background });

    lines.backspace();
    scroll_to_end();
}

void ScrollableScreenPrinter::scroll_up(u16 num_lines) {
    if (lines.count() <= printable_area_height)
        return;

    const u16 FIRST_LINE = 0;
    top_line = (top_line - num_lines <= FIRST_LINE) ? FIRST_LINE : top_line - num_lines;
    redraw();
}

void ScrollableScreenPrinter::scroll_down(u16 num_lines) {
    if (lines.count() <= printable_area_height)
        return;

    const s16 LAST_LINE = lines.count() - printable_area_height;
    top_line = (top_line + num_lines  >= LAST_LINE) ? LAST_LINE : top_line + num_lines;
    redraw();
}

void ScrollableScreenPrinter::scroll_to_begin() {
    if (lines.count() <= printable_area_height)
        return;

    if (top_line == 0)
        return;

    top_line = 0;
    redraw();
}

void ScrollableScreenPrinter::scroll_to_end() {
    if (lines.count() <= printable_area_height)
        return;

    const s16 LAST_LINE = lines.count() - printable_area_height;
    if (top_line == LAST_LINE)
        return;

    top_line = LAST_LINE;
    redraw();
}

bool ScrollableScreenPrinter::is_edit_line_visible() {
    return (s16)lines.count() - printable_area_height <= top_line;
}

/**
 * @brief   Redraw entire printing are. Used when scrolling the text
 */
void ScrollableScreenPrinter::redraw() {
    // calc number of buffer lines to  draw
    u16 lines_to_draw = lines.count() - top_line;
    if (lines_to_draw > printable_area_height)
        lines_to_draw = printable_area_height;

    // draw the lines
    for (u16 y = 0; y < lines_to_draw; y++) 
        put_line_and_clear_remaining_space_at(y, lines[top_line + y]);
   

    // clear remaining lines
    for (u16 y = lines_to_draw; y < printable_area_height; y++) 
        put_line_and_clear_remaining_space_at(y, "");

    draw_scroll_bar();

    cursor.set_visible(is_edit_line_visible());
    cursor.set_x(left + lines.back().length());
    cursor.set_y(top + lines_to_draw -1);

    flush_vga_buffer();
}

void ScrollableScreenPrinter::put_line_and_clear_remaining_space_at(u16 y, const ustd::string& line) {
    u16 num_characters = line.size();

    // print the line
    for (u16 x = 0; x < num_characters; x++) {
        set_at(x, y, VgaCharacter(line[x], foreground, background));
    }

    // clear remaining space of the row
    for (u16 x = num_characters; x < printable_area_width; x++)
        set_at(x, y, VgaCharacter(' ', foreground, background));
}

void ScrollableScreenPrinter::clear_screen() {
    redraw();
}

void ScrollableScreenPrinter::draw_scroll_bar() {
    const u16 BAR_X = right + 1;

    // draw scroll bar dotted background
    for (u16 y = top; y <= bottom; y++)
        set_at(BAR_X, y, VgaCharacter(BG_CHAR, foreground, background));

    // draw actual scrollbar
    u16 bar_size = (printable_area_height > lines.count()) ? printable_area_height : printable_area_height * printable_area_height / lines.count();
    if (bar_size == 0)
        bar_size = 1;

    s16 max_top_line = lines.count() - printable_area_height;
    if (max_top_line == 0)
        max_top_line = 1;

    u16 move_space = printable_area_height - bar_size;
    u16 bar_y = top + move_space * top_line / max_top_line;

    for (u16 y = bar_y; y < bar_y + bar_size; y++)
        set_at(BAR_X, y, VgaCharacter(BG_SCROLLER, foreground, background));
}

void ScrollableScreenPrinter::set_at(const Cursor& cursor, VgaCharacter c) {
    return set_at(cursor.get_x(), cursor.get_y(), c);
}

void ScrollableScreenPrinter::set_at(u16 x, u16 y, VgaCharacter c) {
    vga_buffer[x + y * vga_width] = c;
}

void ScrollableScreenPrinter::flush_vga_buffer() {
    syscalls::vga_flush_char_buffer((u16*)vga_buffer);
}

} // namespace terminal
