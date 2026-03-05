#include "../s21_string.h"

char* s21_strchr(const char* str, int c) {
  char* found = S21_NULL;
  const char* current = str;
  unsigned char search_char = (unsigned char)c;

  for (; *current != '\0' && found == S21_NULL; ++current) {
    if (*current == search_char) {
      found = (char*)current;
    }
  }

  if (found == S21_NULL && search_char == '\0') {
    found = (char*)current;
  }

  return found;
}
