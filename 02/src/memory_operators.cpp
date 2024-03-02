#include "kernel.hpp"
#include "memory.hpp"
#include "memory_operators.hpp"

extern Kernel kernel;

[[nodiscard]] void* operator new(size_t size) noexcept{
    return kernel.memory_manager->allocate(size);
}

[[nodiscard]] void* operator new[](size_t size) noexcept {
    return kernel.memory_manager->allocate(size);
}

void operator delete(void* ptr) noexcept {
    kernel.memory_manager->free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] size_t size)  noexcept {
    kernel.memory_manager->free(ptr);
}


void operator delete[](void* ptr) noexcept {
    kernel.memory_manager->free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] size_t size)  noexcept {
    kernel.memory_manager->free(ptr);
}
