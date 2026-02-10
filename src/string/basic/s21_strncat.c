#include "../s21_string.h"

char* s21_strncat(char* dest, const char* src, s21_size n) {
  char* ptr = dest;

  while (*ptr != '\0') {
    ptr++;
  }

  s21_size i = 0;
  for (; i < n && src[i] != '\0'; ++i) {
    ptr[i] = src[i];
  }

  ptr[i] = '\0';

  return dest;
}
