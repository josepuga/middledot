# Makefile para Middle·Dot
# (c)José Puga 2024. Under GLP3 License.
# Manual GNU/Makefile: https://www.gnu.org/software/make/manual/make.html

# Descomentar para modo debug en vez de compilación de producción
DEBUG	:= 1
# Descomentar para usar los test
TEST 	:= 1
# Descomentar si se quiere detener la compilación si hay warnings
WERROR := 1

TARGET	:= middle
CC		:= gcc
CXX		:= g++
OBJ		:= ./obj
SRC		:= ./src
BIN		:= ./bin
INC		:= $(SRC)/include
ISO		:= ./iso
SRCS	:= $(wildcard $(SRC)/*.cpp)
OBJS	:= $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))

RM		:= rm -f
RMDIR	:= rmdir
MKDIR	:= mkdir

INCFLAGS := -I $(SRC)/include
CXXFLAGS := $(if $(DEBUG), -O0 -g, -O2)
CXXFLAGS := $(CXXFLAGS) $(if $(TEST), -I $(SRC)/test/include -DTEST, )
CXXFLAGS := $(CXXFLAGS) $(if $(WERROR), -Werror, )
CXXFLAGS := $(CXXFLAGS) $(INCFLAGS) -m32 -ffreestanding -nostdlib \
	-nodefaultlibs -fno-exceptions -fno-rtti -mno-red-zone \
	-std=c++23 -Wall -Wextra -Wpedantic

.PHONY: all run clean create_iso debug

default: create_iso

all: clean create_iso run

create_iso : $(TARGET).elf

$(TARGET).elf: $(OBJS) | $(BIN)
	$(CXX) $(CXXFLAGS) -c $(SRC)/start.s -o $(OBJ)/start.o
	@# -lgcc al final del linkado arregla un error: undefined reference to `__umoddi3'
	$(CXX) $(CXXFLAGS) -T $(SRC)/linker.ld $(OBJS) $(OBJ)/start.o -o $(BIN)/$(TARGET).elf -lgcc

create_iso:
	$(shell DEST_ISO=$(ISO) BIN=$(BIN) ./create-iso-and-grub.sh)

$(OBJ)/%.o: $(SRC)/%.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ) $(ISO) $(BIN):
	$(MKDIR) $@

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJ)/start.o
	$(RM) $(BIN)/$(TARGET).elf
	$(shell DEST_ISO=$(ISO) ./delete-iso-and-grub.sh)
# Opcional si se quieren borrar los directorios obj/ y bin/
	if [ -d $(OBJ) ]; then $(RMDIR) $(OBJ); fi
	if [ -d $(BIN) ]; then $(RMDIR) $(BIN); fi

run:
	#qemu-system-i386 -cdrom $(ISO)/middle.iso
	qemu-system-i386 -kernel $(BIN)/$(TARGET).elf

debug:
	$(shell BIN=$(BIN) TARGET=$(TARGET) ./debug-kernel.sh)
