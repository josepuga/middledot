#include <cstddef>
#include <cstdint>
#include <type_traits>
#include "kernel.hpp"
#include "kernel_functions.hpp"
#include "common_functions.hpp"
#include "terminal.hpp"
#ifdef TEST
#include "test-mm.hpp"
#endif

Kernel kernel;


// En lugar de main, usamos esta función. Hay que user extern "C" para evitar
// el mangling.
extern "C" {
void kmain() { 
    MemoryManager mm;
    // El MemoryManager, debe ser global si queremos redefinir los operadores 
    // new/delete para el resto de código del kernel    
    kernel.memory_manager = &mm;

    // Inicializamos con los valores que tenemos defindos por defecto
    if (!mm.init(memory::heap_address, memory::heap_size)) {
        panic("ERROR FATAL: No se pudo inicializar la memoria.");
    }


    Terminal term;
    kernel.terminal = &term;

    term.clear();
    term.set_colors(vga::color::dark_grey, vga::color::black);
    terminal::Position pos;
    pos.col = 2;
    pos.row = 0;
    term.goto_pos(pos);
    term.print_ln("MiddleDot - v0.0.3alpha");
    term.set_colors(vga::color::white, vga::color::black);

#ifdef TEST
    test_memory_manager();
#endif

    for(;;); // No puedes pasar!
}
}
