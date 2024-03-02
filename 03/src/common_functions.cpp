#include "common_functions.hpp"

size_t strlen(const char* str) {
    size_t result {0};
    while (str[result] != 0) {
        result++;
    }
    return result;
}

void reverse_str(char* str) {
    auto len = strlen(str);
    char* start = str;
    char* end = str + len -1;

    while (start < end) {
        char tmp = *start;
        *start++ = *end;
        *end-- = tmp;
    }
}

char* strcpy(char* dest, const char* source) {
    for (size_t i = 0 ;; i++) {
        dest[i] = source[i];
        if (source[i] == '\0') {
            break;
        }
    }
    return dest;
}
