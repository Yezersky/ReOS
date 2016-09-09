//=======================================================================
// Copyright Baptiste Wicht 2013-2016.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://www.opensource.org/licenses/MIT)
//=======================================================================

#include "tlib/graphics.hpp"

namespace {

uint64_t syscall_get(uint64_t call){
    size_t value;
    asm volatile("mov rax, %[call]; int 50; mov %[value], rax"
        : [value] "=m" (value)
        : [call] "r" (call)
        : "rax");
    return value;
}

} // end of anonymous namespace

uint64_t tlib::graphics::get_width(){
    return syscall_get(0x1000);
}

uint64_t tlib::graphics::get_height(){
    return syscall_get(0x1001);
}

uint64_t tlib::graphics::get_x_shift(){
    return syscall_get(0x1002);
}

uint64_t tlib::graphics::get_y_shift(){
    return syscall_get(0x1003);
}

uint64_t tlib::graphics::get_bytes_per_scan_line(){
    return syscall_get(0x1004);
}

uint64_t tlib::graphics::get_red_shift(){
    return syscall_get(0x1005);
}

uint64_t tlib::graphics::get_green_shift(){
    return syscall_get(0x1006);
}

uint64_t tlib::graphics::get_blue_shift(){
    return syscall_get(0x1007);
}

void tlib::graphics::redraw(char* buffer){
    asm volatile("mov rax, 0x1008; mov rbx, %[buffer]; int 50;"
        :
        : [buffer] "g" (buffer)
        : "rax", "rbx");
}

uint64_t tlib::graphics::mouse_x(){
    return syscall_get(0x1100);
}

uint64_t tlib::graphics::mouse_y(){
    return syscall_get(0x1101);
}
