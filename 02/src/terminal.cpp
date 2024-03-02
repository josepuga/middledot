#include "terminal.hpp"


void Terminal::set_colors(const uint8_t fg, const uint8_t bg) {
    fg_color = fg;
    bg_color = bg;
    // Con aritmética binaria se encajan ambos en 8 bits.
    // El primer nibble tendrá el fondo, y el segundo la tinta.
    vga_color = fg | bg << 4;  
}

void Terminal::goto_pos(const terminal::Position& pos) {
    // Overflow check.
    terminal::Position _pos;
    _pos.col = pos.col >= vga::max_cols ? vga::max_cols-1 : pos.col;
    _pos.row = pos.row >= vga::max_rows ? vga::max_rows-1 : pos.row;
    index = vga::max_cols * _pos.row + _pos.col;
}

void Terminal::put_char(const char c) {
    // Gestionar esto requiere que se haga un código que admita "scroll" 
    // y guarde las lineas en un buffer.
    if (index >= vga::buffer_size) {
        return;
    }

    //TODO: Gestionar saltos de linea y llevar control de fila/columna.
    // La entrada es de 16 bits, 8 para los colores y 8 para el carácter.
    uint16_t entry = c;
    if ( c == '\n') {
        terminal::Position pos {get_position()};
        pos.row++;
        pos.col = 0;
        goto_pos(pos);
    } else {
        video_buffer[index] = entry | vga_color << 8;
        index++;
    }
}

terminal::Position Terminal::get_position() {
    terminal::Position result;
    result.row = index / vga::max_cols;
    result.col = index % vga::max_cols;
    return result;
}

void Terminal::clear() {
    index = 0;
    for (size_t n = 0; n < vga::buffer_size; n++) {
        put_char(' ');
    }
    index = 0;
}

void Terminal::print(const char* msg) {
    for (size_t n = 0; msg[n] != 0; n++) {
        // Comprobar que index no pase el buffer de la VGA. Se deja de imprimir
        put_char(msg[n]);
    }
}

void Terminal::print(const char c) {
    put_char(c);
}

void Terminal::print_ln(const char* msg) {
    print(msg);
    put_char('\n');
}


Terminal& Terminal::operator<< (const char* msg){
    print(msg);
    return *this;
}

Terminal& Terminal::operator<< (const char c){
    print(c);
    return *this;
}

Terminal& Terminal::operator<< (terminal::manip m) {
    pipe_use_hex = (m == terminal::manip::hex) ? true: false;
    return *this;
}

