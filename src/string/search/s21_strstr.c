#include "../s21_string.h"

char* s21_strstr(const char* haystack, const char* needle) {
  char* result = S21_NULL;

  if (*needle == '\0') {
    result = (char*)haystack;
  } else {
    s21_size needle_len = s21_strlen(needle);
    s21_size haystack_len = s21_strlen(haystack);
    s21_size i = 0;

    for (i = 0; i + needle_len <= haystack_len && result == S21_NULL; ++i) {
      if (s21_strncmp(haystack + i, needle, needle_len) == 0) {
        result = (char*)(haystack + i);
      }
    }
  }

  return result;
}
