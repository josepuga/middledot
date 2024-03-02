#pragma once
#include <cstddef>

// Funcion necesaria ya que no podemos usar <cstring>
size_t strlen(const char* str);

void reverse_str(char* str);

char* strcpy(char* dest, const char* source);