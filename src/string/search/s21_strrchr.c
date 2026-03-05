#include "../s21_string.h"

char* s21_strrchr(const char* str, int c) {
  char* found = S21_NULL;
  const char* current = str;
  unsigned char search_char = (unsigned char)c;

  for (; *current != '\0'; ++current) {
    if (*current == search_char) {
      found = (char*)current;
    }
  }

  if (search_char == '\0') {
    found = (char*)current;
  }

  return found;
}
