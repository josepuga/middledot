#include "kernel_functions.hpp"
#include "kernel.hpp"

extern Kernel kernel;

[[noreturn]] void panic(const char* msg) {
    kernel.terminal->set_colors(vga::color::white, vga::color::blue);
    kernel.terminal->clear();
    kernel.terminal->print(msg);
    for (;;);
}
