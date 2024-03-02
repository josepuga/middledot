#!/bin/bash
# Middle·Dot - Cleaner del ISO arrancable
# (c)José Puga 2024. Under GLP3 License.

[ -z "$DEST_ISO" ] && echo Este script debe ser llamado por make clean && exit 1

set -euo pipefail

# Peligroso usar rm -fr con /boot/grub!!!!
FILES="middle.iso boot/grub/grub.cfg boot/middle.elf"
DIRS="$DEST_ISO/boot/grub $DEST_ISO/boot $DEST_ISO"
for f in $FILES; do
    [ -f "$DEST_ISO/$f" ] && rm "$DEST_ISO/$f"
done
for d in $DIRS; do
    [ -d "$d" ] && rmdir "$d"
done


