#include "../s21_string.h"

void* s21_memset(void* str, int c, s21_size n) {
  unsigned char* dest_memory = (unsigned char*)str;
  const unsigned char byte_value = (unsigned char)c;
  s21_size i = 0;

  for (; i < n; ++i) {
    dest_memory[i] = byte_value;
  }

  return str;
}
