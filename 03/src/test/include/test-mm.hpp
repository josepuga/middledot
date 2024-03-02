#pragma once 

// Struct de prueba para probar el Memory Manager mediante new/delete 
struct Test {
    char* message1;
    char* message2;
    char* message3;
    Test() {
        message1 = new char[0xff];
        message2 = new char[0xff];
        message3 = new char[0xff];
    }
    ~Test() {
        delete message1;
        delete message2;
        delete message3;
    }
};

// Pruebas de creación de objetos asignados dinámicamente en memoria
// usando new y delete. Para comprobar nuestro Memory Manager.
void test_memory_manager();