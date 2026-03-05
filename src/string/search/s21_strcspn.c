#include "../s21_string.h"

s21_size s21_strcspn(const char* str1, const char* str2) {
  s21_size length = 0;
  const char* str1_ptr = str1;
  const char* str2_ptr = S21_NULL;
  int found = 0;

  for (; *str1_ptr != '\0' && !found; ++str1_ptr) {
    for (str2_ptr = str2; *str2_ptr != '\0' && !found; ++str2_ptr) {
      if (*str1_ptr == *str2_ptr) {
        found = 1;
      }
    }

    if (!found) {
      ++length;
    }
  }

  return length;
}
