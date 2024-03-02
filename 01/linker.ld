/*
  El punto de entrada (ENTRY POINT) donde el bootloader iniciará la ejecución 
  del código, pasando el control al mismo. No tiene porqué llamarse start, pero 
  por convenio suele utilizarse ese nombre casi siempre.
*/
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH("i386")
ENTRY(start)
SECTIONS {

/* https://wiki.osdev.org/User:Zesterer/Bare_Bones
  .text Es donde reside el código ejecutable.
  .rodata Contiene las constantes. Lógicamente es ro. También se puede poner 
        código que no se va a modificar como en este ejemplo el de multiboot.
  .data Contiene las variable INICIALIZADOS en tiempo de compilación de las
        variables globales y las estáticas. Es lectura-escritura (rw).
  .bss (Ver más abajo)
  
  BLOCK() Se usa para alinear la secciones en bloques del tamaño indicado.
  ALIGN() es equivalente, por lo no entiendo la sintaxis bizarra.
*/

    /* Se empiezan las secciones a partir de 1M. Esto es así porque por debajo
       de un 1M, la aquitectura x86 la reserva para usos propios
    */
    . = 1M;
    /* El código de multiboot */
    .multiboot BLOCK(4K) : ALIGN(4K) {
        *(.multiboot)
    }

    /* El código ejecutable (nuestro kernel) */
    .text BLOCK(4K) : ALIGN(4K) {
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

/* 
  .bss Contiene los valores NO INICIALIZADOS de las variables globales 
  (En C/C++, Las variables GLOBALES no inicializ. tienen valor 0), toda
  la sección se rellena con 0. También se usa como espacio para albergar 
  la pila (stack).

  COMMON sólo se usa en los ficheros objeto, no en el ejecutable. Es temporal 
  para casos de definiciones externas de la misma variable. 
  Al crear el ejecutable todo pasa a .bss.
*/
    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }

}
