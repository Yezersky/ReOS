// User Space BMP Library

#include <bmplib.hpp>

void display_bmp(size_t fd, size_t size, const char* file) {
    asm volatile("mov rax, 420; mov rbx, %[fd]; mov rcx, %[size]; mov rdx, %[path]; int 50;"
        : //No outputs
        : [fd] "g" (fd), [size] "g" (size), [path] "g" (reinterpret_cast<size_t>(file))
        : "rax", "rbx", "rcx", "rdx");
}
