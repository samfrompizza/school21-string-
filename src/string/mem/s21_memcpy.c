#include "../s21_string.h"

void* s21_memcpy(void* dest, const void* src, s21_size n) {
  unsigned char* destination = (unsigned char*)dest;
  const unsigned char* source = (const unsigned char*)src;
  s21_size i = 0;

  for (; i < n; ++i) {
    destination[i] = source[i];
  }

  return dest;
}
