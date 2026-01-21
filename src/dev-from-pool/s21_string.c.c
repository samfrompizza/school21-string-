#include "s21_string.h"

size_t s21_strlen(const char *s) {
  size_t len = 0;
  if (!s) return 0;
  while (*s) {
    len++;
    s++;
  }
  return len;
}

int s21_strcmp(const char *s1, const char *s2) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

char *s21_strcpy(char *dest, const char *src) {
  char *p = dest;
  while (*src != '\0') {
    *dest = *src;
    dest++;
    src++;
  }
  *dest = '\0';
  return p;
}

char *s21_strcat(char *dest, const char *src) {
  char *original_dest = dest;

  while (*dest != '\0') {
    dest++;
  }

  while (*src != '\0') {
    *dest = *src;
    dest++;
    src++;
  }

  *dest = '\0';

  return original_dest;
}

char *s21_strchr(const char *str, int c) {
  char ch = (char)c;
  while (*str != '\0') {
    if (*str == ch) {
      return (char *)str;
    }
    str++;
  }

  if (ch == '\0') {
    return (char *)str;
  }

  return NULL;
}

char *s21_strstr(const char *haystack, const char *needle) {
  if (*needle == '\0') {
    return (char *)haystack;
  }

  while (*haystack != '\0') {
    const char *h_ptr = haystack;
    const char *n_ptr = needle;

    while (*n_ptr != '\0' && *h_ptr == *n_ptr) {
      h_ptr++;
      n_ptr++;
    }

    if (*n_ptr == '\0') {
      return (char *)haystack;
    }
    haystack++;
  }

  return NULL;
}

char *s21_strtok(char *str, const char *delim) {
  static char *last_token = NULL;
  char *token_start = NULL;

  if (str == NULL) {
    str = last_token;
    if (str == NULL) {
      return NULL;
    }
  }

  while (*str != '\0' && s21_strchr(delim, *str) != NULL) {
    str++;
  }

  if (*str == '\0') {
    last_token = NULL;
    return NULL;
  }

  token_start = str;

  while (*str != '\0' && s21_strchr(delim, *str) == NULL) {
    str++;
  }

  if (*str != '\0') {
    *str = '\0';
    last_token = str + 1;
  } else {
    last_token = NULL;
  }

  return token_start;
}
