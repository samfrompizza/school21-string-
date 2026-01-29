#include "../s21_string.h"

char* s21_strchr(const char* str, int c) {
  char* result = S21_NULL;
  int found = 0;

  while (*str != '\0' && !found) {
    if (*str == (char)c) {
      result = (char*)str;
      found = 1;
    } else {
      str++;
    }
  }

  if (!found && (char)c == '\0') {
    result = (char*)str;
  }

  return result;
}