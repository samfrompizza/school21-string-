#include "../s21_string.h"

char* s21_strchr(const char* str, int c) {
  char* found = S21_NULL;
  const char* current = str;

  for (; *current != '\0' && found == S21_NULL; ++current) {
    if (*current == (unsigned char)c) {
      found = (char*)current;
    }
  }

  if (found == S21_NULL && (unsigned char)c == '\0') {
    found = (char*)current;
  }

  return found;
}
