#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  char ch = (char)c;

  while (*str != '\0') {
    if (*str == ch) {
      return (char *)str;
    }
    str++;
  }

  if (ch == '\0') {
    return (char *)str;
  }

  return S21_NULL;
}