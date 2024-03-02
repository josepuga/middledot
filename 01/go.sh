#!/bin/bash
# Middle·Dot - Generador del Kernel e ISO arrancable 
# (c)José Puga 2024. Under GLP3 License.
OPTIONS_GPP="-ffreestanding -fno-exceptions -fno-rtti -O2 -mno-red-zone -Wall -Wextra -Wpedantic"
OPTIONS_GCC="-ffreestanding -nostdlib -nodefaultlibs -mno-red-zone -Wall -Wextra -Wpedantic" # -lgcc

DEST_ISO="./iso"
GRUB_ENTRY='
GRUB_GFXPAYLOAD_LINUX=text
menuentry "MiddleDot" {
    GRUB_TERMINAL=console
    multiboot /boot/middle.elf
}
'

set -euo pipefail
g++ -m32 -c start.s -o start.o $OPTIONS_GCC
g++ -m32 -c kernel.cpp -o kernel.o $OPTIONS_GPP
g++ -m32 start.o kernel.o -T linker.ld -o middle.elf $OPTIONS_GCC

[ ! -d "$DEST_ISO/boot/grub" ] && mkdir -p "$DEST_ISO/boot/grub"
# grub2-mkrescue xorriso termina con error si sobrescribe el iso :O
[ -f "$DEST_ISO/middle.iso" ] && rm "$DEST_ISO/middle.iso"
cp middle.elf "$DEST_ISO/boot"
echo "$GRUB_ENTRY" > "$DEST_ISO/boot/grub/grub.cfg"
grub2-mkrescue --output="$DEST_ISO/middle.iso" "$DEST_ISO"

