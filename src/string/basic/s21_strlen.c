#include "../s21_string.h"

s21_size s21_strlen(const char* str) {
  s21_size len = 0;

  while (str[len]) {
    len++;
  }

  return len;
}