#include <stdlib.h>

#include "../string/s21_string.h"

static int char_in_set(char c, const char *set) {
  if (set == S21_NULL) return 0;
  for (s21_size i = 0; set[i] != '\0'; ++i)
    if (set[i] == c) return 1;
  return 0;
}

void *s21_trim(const char *src, const char *trim_chars) {
  if (src == S21_NULL) return S21_NULL;

  const char *default_ws = " \t\n\v\f\r";
  const char *set = (trim_chars == S21_NULL) ? default_ws : trim_chars;

  s21_size src_len = s21_strlen(src);
  s21_size left = 0;
  while (left < src_len && char_in_set(src[left], set)) left++;

  if (left == src_len) {
    char *empty = calloc(1, sizeof(char));
    if (empty == S21_NULL) return S21_NULL;
    empty[0] = '\0';
    return (void *)empty;
  }

  s21_size right = src_len - 1;
  while (right > left && char_in_set(src[right], set)) right--;

  s21_size new_len = right - left + 1;
  char *new_str = calloc(new_len + 1, sizeof(char));
  if (new_str == S21_NULL) return S21_NULL;

  for (s21_size i = 0; i < new_len; ++i) new_str[i] = src[left + i];
  new_str[new_len] = '\0';
  return (void *)new_str;
}