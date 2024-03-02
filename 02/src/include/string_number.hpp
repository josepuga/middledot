#pragma once
#include <type_traits>
#include <utility>
#include "common_functions.hpp"

// Imprime cualquier entero como una cadena en formato hexadecimal 0x...
// use_max_digits: Adapta la longitud a los bytes que ocupa la variable.
template <class T, typename std::enable_if<
    std::is_integral<T>::value, int>::type = 0>
void string_hex(const T n, char* buffer, const bool use_max_digits = false);
// Imprime cualquier entero como una cadena
template <class T, typename std::enable_if<
    std::is_integral<T>::value, int>::type = 0>
void string_int(const T n, char* buffer);

// Idem. con floats, y se puede indicar precisión.
template <class T, typename std::enable_if<
    std::is_floating_point<T>::value, int>::type = 0>
void string_float(const T n, char* buffer, const unsigned int precision = 2);


// ----------------------------------------------------------------------------
// IMPLEMENTACIÓN.
// Plantillas deben implementarse en el fichero de cabecera.



template <class T, typename std::enable_if<
    std::is_integral<T>::value, int>::type>
void string_int(const T n, char* buffer) {

    size_t i {0};
    auto abs_n {n};
    // Se convierte a valor absoluto si fuera negativo
    if (n < 0) {
        abs_n *= -1;
    }
    // conversión del carácter, sumándo al módulo el ASCII '0'.
    while (abs_n != 0) {
        buffer[i++] = '0' + (abs_n % 10);
        abs_n /= 10;
    }
    // Se añade el signo menos si negativo.
    if ( n < 0) {
        buffer[i++] = '-';
    }
    buffer[i++] = '\0';

    // Ahora tenemos un bonito número, pero al revés. Hay que revertirlo.
    reverse_str(buffer);
}


template<class T, typename std::enable_if<
    std::is_integral<T>::value, int>::type>
void string_hex(const T n, char* buffer, const bool use_max_digits) {

    auto constexpr hex_chars = "0123456789ABCDEF";

    // Tratamos los negativos como positivos.
    auto num {n};
    if ( num < 0) {
        num *= -1;
    }

    // Ancho máximo por defecto
    size_t max_digits = sizeof(T) * 2;
    if (!use_max_digits) {
        // Calcular el ancho real sin ceros a la izda.
        max_digits = 0;
        T tmp = num;
        while (tmp > 0) {
            tmp /= 16;
            max_digits++;
        }
    }

    // Mínimo un dígito
    if (max_digits == 0) {
        max_digits++;
    }

    // Versión corregida usando aritmética binaria.
    // Cada byte se desplaza a la dcha 4 bits se hace un AND al nibble
    // para extraer el carácter. El bucle es "* 2" porque cada byte
    // se trata 2 veces.
    size_t i {0};
    for (size_t n = 0; n < max_digits; n++) {
        buffer[i++] = hex_chars[(num >> (4 * n)) & 0xf];
    }

    buffer[i++] = 'x';
    buffer[i++] = '0';
    buffer[i] = '\0';

    reverse_str(buffer);
}


template <class T, typename std::enable_if<
    std::is_floating_point<T>::value, int>::type>
void string_float(const T n, char* buffer, const unsigned int precision){

    auto is_negative {n < 0 ? true: false};
    /*
    auto abs_n { is_negative ? static_cast<unsigned long long>(n * -1)
            : static_cast<unsigned long long>(n)};
    */
    auto abs_n {is_negative ? -n : n};
    auto integer_part {static_cast<unsigned long long>(abs_n)};
    T decimal_part {abs_n - static_cast<T>(integer_part)};
    size_t i {0};

    // Parte entera
    
    while (integer_part != 0) {
        buffer[i++] = '0' + (integer_part % 10);
        integer_part /= 10;
    }
    // Si el número es negativo, añadir signo.
    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';
    reverse_str(buffer);

    // Parte decimal
    if (precision > 0) {
        buffer[i++] = '.';
        unsigned int decimal_places = 0;
        while (decimal_places < precision) {
            decimal_part *= 10;
            buffer[i++] = '0' + static_cast<unsigned long long>(decimal_part);
            decimal_part -= static_cast<T>(static_cast<unsigned long long>(decimal_part));
            ++decimal_places;
        }
    }
    buffer[i] = '\0';
}
