#include "../s21_string.h"

char* s21_strrchr(const char* str, int c) {
  char* found = S21_NULL;
  const char* current = str;

  for (; *current != '\0'; ++current) {
    if (*current == (unsigned char)c) {
      found = (char*)current;
    }
  }

  if ((unsigned char)c == '\0') {
    found = (char*)current;
  }

  return found;
}
