#!/bin/bash
# Middle·Dot - Depurador del Kernel con QEMU y gcc.
# (c)José Puga 2024. Under GLP3 License.

# Poner a 1 si se quiere depurar usando Visual Studio Code.
USE_VSCODE=1

[ -z "$TARGET" ] && echo Este script debe ser llamado por make debug && exit 1
QEMU_ARGS=(-s -S -kernel "$BIN/$TARGET.elf")
GDB_ARGS=("$BIN/$TARGET.elf" -tui -ex "target remote localhost:1234")

# NOTA1: Aquí no sirve: set -euo pipefail

# NOTA2: A mí, no me respondía el teclado si usaba gdb en la misma consola que
#        lanzaba qemu. Me veo forzado a abrir una nueva consola. Se puede hacer
# desde la misma consola con gdb "$GDB_ARGS" &
# Si también te pasara el bloqueo del teclado y no usaras KDE, mira la 
# documetación de tu distro 
qemu-system-i386 "${QEMU_ARGS[@]}" &
[[ "$USE_VSCODE" != 1 ]] && konsole -e gdb "${GDB_ARGS[@]}" &