#include <stdio.h>

#ifndef S21_STRING_H
#define S21_STRING_H

size_t s21_strlen(const char *s);
int s21_strcmp(const char *s1, const char *s2);
char *s21_strcpy(char *dest, const char *src);
char *s21_strcat(char *dest, const char *src);
char *s21_strchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);

#endif
