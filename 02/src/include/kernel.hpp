#pragma once
#include "terminal.hpp"
#include "memory.hpp"
// Esta estructura la usarán las diferentes partes para referirse a cualquier
// objeto creado en el kernel.
struct Kernel {

    // No puede crease con new!!!
    void* operator new(size_t) = delete;
    void* operator new[](size_t) = delete;

    Terminal* terminal;
    MemoryManager* memory_manager;
};
