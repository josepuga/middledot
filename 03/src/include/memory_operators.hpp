#pragma once
#include "memory.hpp"


// Sobreescribimos los operadores new/delete para que usen el gestor de memoria
// del kernel.
// NOTA: nodiscard se incorpora en c++20
[[nodiscard]] void* operator new(size_t size) noexcept;
[[nodiscard]] void* operator new[](size_t size) noexcept;
void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

// El compilador me obliga a tener este operador delete con tamaño (C++20). No
// se le dará uso, tan sólo llamará al "normal"
void operator delete(void* ptr, [[maybe_unused]] size_t size) noexcept;
void operator delete[](void* ptr, [[maybe_unused]] size_t size) noexcept;

