#include "../string/s21_string.h"

#include <stdlib.h>

void *s21_insert(const char *src, const char *str, s21_size start_index) {
  if (src == S21_NULL || str == S21_NULL) return S21_NULL;

  s21_size src_len = s21_strlen(src);
  s21_size str_len = s21_strlen(str);

  if (start_index > src_len) return S21_NULL;

  s21_size new_len = src_len + str_len;
  char *new_str = calloc(new_len + 1, sizeof(char));
  if (new_str == S21_NULL) return S21_NULL;

  for (s21_size i = 0; i < start_index; ++i) new_str[i] = src[i];

  for (s21_size i = 0; i < str_len; ++i) new_str[start_index + i] = str[i];

  for (s21_size i = start_index; i < src_len; ++i) new_str[str_len + i] = src[i];

  new_str[new_len] = '\0';
  return (void *)new_str;
}