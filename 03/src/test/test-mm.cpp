#include "test-mm.hpp"
#include "kernel.hpp"
#include "terminal.hpp"

extern Kernel kernel;

void test_memory_manager(){
    auto term {kernel.terminal};

    auto var_local1 = 125;
    auto var_local2 = 3982;
    Test* test1 = new(Test); //Le asignamos memoria dinámica :) !!!!

    // Nuestra implementación de strcpy. (Ambas asignaciones correctas)
    test1->message1 = strcpy(test1->message1, "Probando el Memory Manager....");
    strcpy(test1->message2, "abcdefghijklmnopqrstuvwxyz!@@@");
    strcpy(test1->message3, "El mensaje 3 ha sido asignado.");

    term->format_ln("test1 = %H", reinterpret_cast<uintptr_t>(test1));
    term->format_ln("var_local1 = %H", reinterpret_cast<uintptr_t>(&var_local1));
    term->format_ln("var_local2 = %H", reinterpret_cast<uintptr_t>(&var_local2));
    term->format_ln("t1->message1 = %s, Mem. = %H", test1->message1, reinterpret_cast<uintptr_t>(test1->message1));
    term->format_ln("t1->message2 = %s, Mem. = %H", test1->message2, reinterpret_cast<uintptr_t>(test1->message2));
    term->format_ln("t1->message3 = %s, Mem. = %H", test1->message3, reinterpret_cast<uintptr_t>(test1->message3));

    // Al borrar el objeto, su memoria se libera...
    delete test1;
    test1 = nullptr; // Buena práctica hacer esto.

    // ...el nuevo objeto, coge la memoria anterior liberada.
    Test* test2 = new(Test);
    strcpy(test2->message1, "Misma dir. que test1->message1");
    term->format_ln("\ntest2 = %H", reinterpret_cast<uintptr_t>(test2));
    term->format_ln("t2->message1 = %s, Mem. = %H", test2->message1, reinterpret_cast<uintptr_t>(test2->message1));
}