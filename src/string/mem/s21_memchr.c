#include "../s21_string.h"

void* s21_memchr(const void* str, int c, s21_size n) {
  const unsigned char* s = (const unsigned char*)str;
  unsigned char ch = c;
  void* result = s21_NULL;

  for (s21_size i = 0; i < n; i++) {
    if (s[i] == ch && result == s21_NULL) {
      result = (void*)(s + i);
    }
  }

  return result;
}