#include "../s21_string.h"

char* s21_strncpy(char* dest, const char* src, s21_size n) {
  s21_size i;

  for (i = 0; i < n && src[i] != '\0'; i++) {
    dest[i] = src[i];
  }

  for (; i < n; i++) {
    dest[i] = '\0';
  }

  return dest;
}