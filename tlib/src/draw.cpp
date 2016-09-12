#include <draw.hpp>

void draw() {
    asm volatile("mov rax, 410; int 50"
        : //No outputs
        : //No inputs
        : "rax");
}
