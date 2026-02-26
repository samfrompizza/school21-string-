#include "../s21_string.h"

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
    if (s21_strchr(delimiters, (unsigned char)*current_pos) == S21_NULL) {
      found_token = current_pos;
    }
  }

  if (found_token != S21_NULL) {
    int token_detected = 0;

    for (; *current_pos != '\0' && !token_detected; ++current_pos) {
      if (s21_strchr(delimiters, (unsigned char)*current_pos) != S21_NULL) {
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