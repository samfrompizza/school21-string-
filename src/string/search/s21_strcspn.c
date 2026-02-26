#include "../s21_string.h"

s21_size s21_strcspn(const char* str1, const char* str2) {
  s21_size span = 0;

  for (; str1[span] != '\0'; ++span) {
    if (s21_strchr(str2, (unsigned char)str1[span]) != S21_NULL) {
      return span;
    }
  }

  return span;
}
