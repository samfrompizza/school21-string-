#include "s21_parse_helpers.h"

inline int s21_isdigit(int ch) { return ch >= '0' && ch <= '9'; }

inline int s21_isspace(int ch) {
  return ch == ' ' || ch == '\f' || ch == '\n' || ch == '\r' || ch == '\t' ||
         ch == '\v';
}

inline int s21_isoctal(int ch) { return ch >= '0' && ch <= '7'; }

inline int s21_isxdigit_parse(int ch) {
  return s21_isdigit(ch) || (ch >= 'a' && ch <= 'f') ||
         (ch >= 'A' && ch <= 'F');
}

inline int s21_xdigit_value(int ch) {
  if (ch >= '0' && ch <= '9') {
    return ch - '0';
  }
  if (ch >= 'a' && ch <= 'f') {
    return ch - 'a' + 10;
  }
  if (ch >= 'A' && ch <= 'F') {
    return ch - 'A' + 10;
  }
  return -1;
}

inline void s21_skip_whitespace(const char** p) {
  while (**p && s21_isspace((unsigned char)**p)) {
    (*p)++;
  }
}
