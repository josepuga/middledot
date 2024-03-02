// https://wiki.osdev.org/User:Zesterer/Bare_Bones

// Nuestro "main" que definimos como función principal en kernel.cpp
.extern kmain

// Indicamos el símbolo (label) start, que también indicamos en el linker.
.global start

// Grub necesita una información básica conocida como "Multiboot Header"
// Esta lineas la crean
.set MB_MAGIC, 0x1BADB002
.set MB_FLAGS, (1 << 0) | (1 << 1)
.set MB_CHECKSUM, (0 - (MB_MAGIC + MB_FLAGS))

// Esta es la sección del ejecutable que contiene la Cabecera Multiboot
// He tenido que poner este "hack" para que la sección tuviera los atributos AX
// igual que .text. (readelf -S middle.elf)
.section .multiboot, "ax", @progbits
    .align 4
    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECKSUM

.section .bss
    .align 16
    stack_bottom:
        // 32 KiB
        .skip 32768
    stack_top:

.section .text
    start:
        call kmain
