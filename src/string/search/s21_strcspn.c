#include "../s21_string.h"

s21_size s21_strcspn(const char* str1, const char* str2) {
  s21_size len = 0;
  int found = 0;

  while (str1[len] != '\0' && !found) {
    s21_size j = 0;
    while (str2[j] != '\0' && !found) {
      if (str1[len] == str2[j]) {
        found = 1;
      }
      j++;
    }

    if (!found) {
      len++;
    }
  }

  return len;
}