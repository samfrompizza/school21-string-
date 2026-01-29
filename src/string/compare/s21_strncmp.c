#include "../s21_string.h"

int s21_strncmp(const char* str1, const char* str2, s21_size n) {
  int result = 0;
  s21_size i = 0;
  int found_diff = 0;

  while (i < n && !found_diff) {
    if (str1[i] != str2[i]) {
      result = (unsigned char)str1[i] - (unsigned char)str2[i];
      found_diff = 1;
    } else if (str1[i] == '\0') {
      found_diff = 1;
    } else {
      i++;
    }
  }

  return result;
}