#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdarg>
#include <type_traits>
#include <limits>
#include <utility>
#include "common_functions.hpp"
#include "memory_operators.hpp"
#include "string_number.hpp"

// Constantes para manejar la VGA
namespace vga {

// Paleta de color: https://www.fountainware.com/EXPL/vga_color_palettes.htm
enum color : uint8_t { 
    black = 0,
    blue = 1,
    green = 2,
    cyan = 3,
    red = 4,
    magenta = 5,
    brown = 6,
    light_grey = 7,
    dark_grey = 8,
    light_blue = 9,
    light_green = 10,
    light_cyan = 11,
    light_red = 12,
    light_magenta = 13,
    yellow = 14, //light_brown?
    white = 15,
};

// Límites
constexpr uint8_t max_rows = 25;
constexpr uint8_t max_cols = 80;
constexpr size_t buffer_size = max_rows * max_cols;

// Esta es la dirección de memoria inicial con un buffer 16bits de 80*24 de 
// tamaño. Cualquier cosa que se escriba aquí, la BIOS se encarga de mostrarlo
// en pantalla.
constexpr size_t memory_start = 0xB8000;

} // namespace


namespace terminal {
    enum index : uint8_t {
        current = 0,    // Siempre apuntará a la terminal activa.
        num1 = 1,
        num2 = 2,
        num3 = 3,
        num4 = 4,
    };

    struct Position {
        size_t col {0}, row {0};
    };

    // Estos manipuladores pretender funcionar similiar a los de iostream/iomanip
    // De momento se implementan para mostrar los valores en decimal o hexadec.
    enum class manip {
        reset = 0,  // TODO: Reseteará cualquier manipulador activo (futuro uso).
        dec = 1,
        hex = 2,
    };
    // Usado para crear los buffers cuando se imprime un número como string
    constexpr size_t string_number_size = 20;
}

// Manejamos la pantalla VGA con una clase Terminal, mediante funciones 
// rudimentarias, podremos escribir texto en la pantalla
class Terminal {
    private:
        uint8_t fg_color {vga::color::light_grey};
        uint8_t bg_color {vga::color::black};
        uint8_t vga_color {0};
        uint16_t *video_buffer {(uint16_t*) vga::memory_start};
        size_t index {0}; //Buffer index
        bool pipe_use_hex {0};  // Para mostrar hex. a través de "<<"

        void put_char(const char);

        // Estas funciones son necesarias internamente para nuestra func. format()
        // La primera, es llamada al final cuando no hay más argumentos, termina de
        // imprimir el resto de la cadena.
        // %d = Imprime número entero base decimal (10).
        // %f = Imprime número real o float
        // %h = Imprime número entero. en hexadecimal
        // %H = Idem. pero rellena con ceros los dígitos que ocupa dicho número
        template<typename... Args>
        void _format(const char*& s);
        template<typename T, typename... Args>
        void _format(const char*& s, T&& value, Args&&... args);  

    public:
        // Posiciona el cursor en la columna x, fila y.
        void goto_pos(const terminal::Position&);

        // Asigna los colores de tinta y fondo.
        void set_colors(const uint8_t fg, const uint8_t bg);

        // Imprime una cadena en la posición actual con el color actual
        void print(const char*);

        void print(const char);

        // Como print(), pero hace un salto de linea.
        void print_ln(const char*);

        // Imprime cualquier entero como una cadena
        template <class T, typename std::enable_if<
            std::is_integral<T>::value, int>::type = 0>
        void print(const T n);

        // Entero como hexadecimal
        template <class T, typename std::enable_if<
            std::is_integral<T>::value, int>::type = 0>
        void print_hex(const T n, const bool use_max_digits = false);

        // Idem. con floats, y se puede indicar precisión.
        template <class T, typename std::enable_if<
            std::is_floating_point<T>::value, int>::type = 0>
        void print(const T n, unsigned int precision = 2);

        // Versión adaptada. Similar a printf() de C. o std::format() de C++
        template<typename... Args>
        void format(const char* str, Args&&... args);
        template<typename... Args>
        void format_ln(const char* str, Args&&... args);

        // Sobrecarga del operador << para poder usarlo estilo "std::cout <<"
        Terminal& operator<< (const char*);
        Terminal& operator<< (const char);
        Terminal& operator<< (terminal::manip);
        template <class T, typename std::enable_if<
            std::is_integral<T>::value, int>::type = 0>
        Terminal& operator<< (const T& n);

        // Borra la pantalla con los colores actuales.
        void clear();

        terminal::Position get_position();
};

// Las plantillas tienen que estar en el header.

template<class T, typename std::enable_if<
    std::is_integral<T>::value, int>::type>
void Terminal::print(const T n) {
    char *buffer = new char[terminal::string_number_size];
    string_int(n, buffer);
    print(buffer);
    delete[] buffer;
}

template <class T, typename std::enable_if<
    std::is_floating_point<T>::value, int>::type>
void Terminal::print(const T n, unsigned int precision) {
    char *buffer = new char[terminal::string_number_size];
    string_float(n, buffer, precision);
    print(buffer);
    delete[] buffer;
}

template<class T, typename std::enable_if<
    std::is_integral<T>::value, int>::type>
void Terminal::print_hex(const T n, const bool use_max_digits) {
    char *buffer = new char[terminal::string_number_size];
    string_hex(n, buffer, use_max_digits);
    print(buffer);
    delete[] buffer;
}

template <class T, typename std::enable_if<
    std::is_integral<T>::value, int>::type>
Terminal& Terminal::operator<< (const T& n) {
    //T value = std::forward<T>(n);
    if (pipe_use_hex) {
        print_hex(n);
    } else {
        print(n);
    }
    return *this;
}

template<typename... Args>
void Terminal::_format([[maybe_unused]] const char*& s) {
    print(s);
}

template<typename T, typename... Args>
void Terminal::_format(const char*& s, T&& value, Args&&... args) {
    while (*s != '\0' && *s != '%') {
        print(*s);
        s++;
    }
    if (*s == '%') {
        // lvalue y rvalue se comportan de forma diferente. Con esto extraemos
        // el tipo subyacente.
        using base_type = typename std::remove_reference<T>::type;
        switch (*(s + 1)) {
            case 'd':   // Formato decimal
                if constexpr (std::is_integral<base_type>::value) {//std::is_same_v<std::decay_t<T>, int>) {
                    print(value); // print(int)
                }
                break;
            case 'h':   // Formato hexadecimal (dígitos mínimos)
                if constexpr (std::is_integral<base_type>::value) {
                    print_hex(value, false);
                }
                break;
            case 'H':   // Formato hexadecimal (todos los dígitos)
                if constexpr (std::is_integral<base_type>::value) {
                    print_hex(value, true);
                }
                break;
            case 'f':   // Float
                if constexpr (std::is_floating_point<base_type>::value){ //std::is_same_v<std::decay_t<T>, float>) {
                    print(value); // print(float)
                } else if constexpr (std::is_integral<base_type>::value) {
                    print(value); // print(int)
                }
                break;
            case 's':   // Cadena de texto
                print(value);
                break;
            case '%':  // Escape del signo '%', Imprimimos sólo un '%'
                print(*s);
                break;
            default:   // No hay nada más implementado de momento.
                print(*s);
                print(*(s + 1));
                break;
        }
        s += 2;  // Avanzar solo después de procesar un marcador de formato
    }
    _format(s, std::forward<Args>(args)...);
}

template<typename... Args>
void Terminal::format(const char* str, Args&&... args) {
    auto s = str;
    _format(s, std::forward<Args>(args)...);
}

template<typename... Args>
void Terminal::format_ln(const char* str, Args&&... args) {
    format(str, std::forward<Args>(args)...);
    print('\n');
}
