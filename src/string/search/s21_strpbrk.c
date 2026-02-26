#include "../s21_string.h"

char* s21_strpbrk(const char* str1, const char* str2) {
  for (; *str1 != '\0'; ++str1) {
    if (s21_strchr(str2, (unsigned char)*str1) != S21_NULL) {
      return (char*)str1;
    }
  }
  return S21_NULL;
}
