#include "../s21_string.h"

char* s21_strstr(const char* haystack, const char* needle) {
  char* result = S21_NULL;
  int found = 0;

  if (*needle == '\0') {
    result = (char*)haystack;
    found = 1;
  }

  for (const char* h = haystack; *h != '\0' && !found; h++) {
    const char* h_temp = h;
    const char* n = needle;

    while (*h_temp != '\0' && *n != '\0' && *h_temp == *n) {
      h_temp++;
      n++;
    }

    if (*n == '\0') {
      result = (char*)h;
      found = 1;
    }
  }

  return result;
}