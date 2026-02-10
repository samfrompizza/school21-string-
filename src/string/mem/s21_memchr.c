#include "../s21_string.h"

void* s21_memchr(const void* str, int c, s21_size n) {
  const unsigned char* search_string = (const unsigned char*)str;
  unsigned char search_char = (unsigned char)c;
  s21_size i = 0;
  void* result = S21_NULL;

  for (; i < n && result == S21_NULL; ++i) {
    if (search_string[i] == search_char) {
      result = (void*)(search_string + i);
    }
  }

  return result;
}
