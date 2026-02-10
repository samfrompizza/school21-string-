#include "../s21_string.h"

char* s21_strpbrk(const char* str1, const char* str2) {
  char* result = S21_NULL;
  const char* s1;
  const char* s2;

  for (s1 = str1; *s1 != '\0' && result == S21_NULL; ++s1) {
    for (s2 = str2; *s2 != '\0' && result == S21_NULL; ++s2) {
      if (*s1 == *s2) {
        result = (char*)s1;
      }
    }
  }

  return result;
}
