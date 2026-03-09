#include "../s21_string.h"

int s21_strncmp(const char* str1, const char* str2, s21_size n) {
  int result = 0;
  s21_size i = 0;

  for (; i < n; ++i) {
    unsigned char char1 = (unsigned char)str1[i];
    unsigned char char2 = (unsigned char)str2[i];

    if (char1 != char2 || char1 == '\0') {
      result = char1 - char2;
      i = n;
    }
  }

  return result;
}
