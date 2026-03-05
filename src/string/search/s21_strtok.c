#include "../s21_string.h"

static int s21_is_delimiter(char c, const char* delimiters) {
  int result = 0;

  for (const char* delim_ptr = delimiters; *delim_ptr != '\0' && !result;
       ++delim_ptr) {
    if (c == *delim_ptr) {
      result = 1;
    }
  }

  return result;
}

char* s21_strtok(char* str, const char* delimiters) {
  static char* saved_position = S21_NULL;
  char* found_token = S21_NULL;
  char* current_pos;

  if (str != S21_NULL) {
    current_pos = str;
  } else {
    current_pos = saved_position;
  }

  if (current_pos == S21_NULL) {
    return S21_NULL;
  }

  for (; *current_pos != '\0' && found_token == S21_NULL; ++current_pos) {
    if (!s21_is_delimiter(*current_pos, delimiters)) {
      found_token = current_pos;
    }
  }

  if (found_token != S21_NULL) {
    int token_detected = 0;

    for (; *current_pos != '\0' && !token_detected; ++current_pos) {
      if (s21_is_delimiter(*current_pos, delimiters)) {
        *current_pos = '\0';
        saved_position = current_pos + 1;
        token_detected = 1;
      }
    }

    if (!token_detected) {
      saved_position = S21_NULL;
    }
  }

  return found_token;
}
