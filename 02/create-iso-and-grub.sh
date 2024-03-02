#!/bin/bash
# Middle·Dot - Generador del Kernel e ISO arrancable
# (c)José Puga 2024. Under GLP3 License.

[ -z "$DEST_ISO" ] && echo Este script debe ser llamado por make && exit 1
GRUB_ENTRY='
GRUB_GFXPAYLOAD_LINUX=text
menuentry "MiddleDot" {
    GRUB_TERMINAL=console
    multiboot /boot/middle.elf
}
'

set -euo pipefail
[ ! -d "$DEST_ISO/boot/grub" ] && mkdir -p "$DEST_ISO/boot/grub"
# grub2-mkrescue xorriso termina con error si sobrescribe el iso :O
[ -f "$DEST_ISO/middle.iso" ] && rm "$DEST_ISO/middle.iso"
cp "$BIN/middle.elf" "$DEST_ISO/boot"
echo "$GRUB_ENTRY" > "$DEST_ISO/boot/grub/grub.cfg"
grub2-mkrescue --output="$DEST_ISO/middle.iso" "$DEST_ISO"
