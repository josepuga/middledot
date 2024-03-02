#include "test-03.hpp"
#include "kernel.hpp"
#include "terminal.hpp"

extern Kernel kernel;

// Ejemplo de impresión "clásica", y uso de format.
void test_print_classic() {
    auto term {kernel.terminal};
    term->clear();

    // Al objeto Terminal le pasamos un objeto Position para controlar la
    // posición de impresión
    terminal::Position pos;  // Tiene 2 campos: col y row.
    pos.col = 2;
    pos.row = 4;
    term->goto_pos(pos);
    term->format("Formateo de texto en columna %d, fila %d", pos.col, pos.row);

    pos.row++;
    term->goto_pos(pos);
    term->print_ln("Texto en linea inferior + salto de linea");

    term->format_ln("Número 255 en hexadecimal: %h", 255); // Ojo acentos no funcionan

    // Prueba de longitud de caracteres a mostrar según el tipo
    uint64_t my_hex {0x231};
    term->format_ln("Hex 'normal': %h. Hex 'completo': %H",my_hex, my_hex);

    int value {123589};
    term->print_ln("Texto con salto de linea");
    term->print("Mostrar variable o numero en formato decimal: ");
    term->print(value);
}


// Ejemplo con la sintaxis << de C++. No hace falta teclear term->print()
void test_print_pipe(){
    auto term {kernel.terminal};
    int value {1223589};
    term->clear();
    terminal::Position pos;  // Tiene 2 campos: col y row.
    pos.col = 3;
    pos.row = 8;
    term->goto_pos(pos);
    *term << "Esto es similar a imprimir al estilo C++, podemos " <<
        "imprimir numeros y variables " << 1980 << " " << value << '\n';
    *term << "Los valores se pueden imprimir en hexadecimal usando un manipulador " <<
        terminal::manip::hex << 
        value << " " << 1980 << ". Con los manipuladores hex/dec alternamos. " << 
        terminal::manip::dec <<
        value << " " << 1980 << ". Ahora se muestran otra vez en decimal.\n";

    
}

void test_memory_manager(){
    auto term {kernel.terminal};
    *term << terminal::manip::hex; // Vamos a mostrar todo en hexadecimal.

    Test* test1 = new(Test); //Le asignamos memoria dinámica :) !!!!

    // Nuestra implementación de strcpy. (Ambas asignaciones correctas)
    test1->message1 = strcpy(test1->message1, "Probando el Memory Manager...");
    strcpy(test1->message2, "ABCDEFGHIJKLMNOPQRSTUVZabcdefghijklmnopqrstuvwxyz");
    strcpy(test1->message3, "El mensaje 3 tambien ha sido asignado.");


    // Es necesaria esta conversión para mostrar direcciones de memoria.
    auto addr {reinterpret_cast<uintptr_t>(test1->message1)};   
    *term << "test1->message1: " << addr << " - " << test1->message1 << '\n';
    addr = reinterpret_cast<uintptr_t>(test1->message2);
    *term << "test1->message2: " << addr << " - " << test1->message2 << '\n';
    addr = reinterpret_cast<uintptr_t>(test1->message3);
    *term << "test1->message3: " << addr << " - " << test1->message3 << '\n';

    // Al borrar el objeto, su memoria se libera...
    delete test1;
    test1 = nullptr; // Buena práctica hacer esto.

    // ...el nuevo objeto, coge la memoria anterior liberada.
    Test* test2 = new(Test);

    strcpy(test2->message1, "Misma direccion que tenia test1->message1");
    addr = reinterpret_cast<uintptr_t>(test2->message1);
    // Probamos a imprimir con format()...
    term->format("\ntest2->message1: %h - %s", addr, test2->message1);
}