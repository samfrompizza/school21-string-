#include "../s21_string.h"

char* s21_strpbrk(const char* str1, const char* str2) {
  char* result = S21_NULL;

  while (*str1 != '\0' && result == S21_NULL) {
    const char* check = str2;
    while (*check != '\0' && result == S21_NULL) {
      if (*str1 == *check) {
        result = (char*)str1;
      }
      check++;
    }
    str1++;
  }

  return result;
}