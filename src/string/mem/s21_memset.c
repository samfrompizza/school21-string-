#include "../s21_string.h"

void* s21_memset(void* str, int c, s21_size n) {
  unsigned char* s = (unsigned char*)str;
  unsigned char ch = (unsigned char)c;

  for (s21_size i = 0; i < n; i++) {
    s[i] = ch;
  }

  return str;
}