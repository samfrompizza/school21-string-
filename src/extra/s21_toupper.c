#include <stdlib.h>

#include "../string/s21_string.h"

void *s21_to_upper(const char *str) {
  if (str == S21_NULL) return S21_NULL;

  s21_size len = s21_strlen(str);
  char *new_str = calloc(len + 1, sizeof(char));
  if (new_str == S21_NULL) return S21_NULL;

  for (s21_size i = 0; i < len; ++i) {
    unsigned char c = (unsigned char)str[i];
    if (c >= 'a' && c <= 'z')
      new_str[i] = (char)(c - ('a' - 'A'));
    else
      new_str[i] = (char)c;
  }

  new_str[len] = '\0';
  return (void *)new_str;
}