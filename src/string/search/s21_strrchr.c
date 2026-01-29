#include "../s21_string.h"

char* s21_strrchr(const char* str, int c) {
  char* result = S21_NULL;
  const char* current = str;

  while (*current != '\0') {
    if (*current == (char)c) {
      result = (char*)current;
    }
    current++;
  }

  if ((char)c == '\0') {
    result = (char*)current;
  }

  return result;
}