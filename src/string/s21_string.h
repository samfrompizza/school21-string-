#ifndef S_21_STRING_H_
#define S_21_STRING_H_

#define S21_NULL ((void*)0)

typedef unsigned long long s21_size;

/* Part 1: string.h library functions (README spec) */
extern void* s21_memchr(const void* str, int c, s21_size n);
extern int s21_memcmp(const void* str1, const void* str2, s21_size n);
extern void* s21_memcpy(void* dest, const void* src, s21_size n);
extern void* s21_memset(void* str, int c, s21_size n);
extern char* s21_strncat(char* dest, const char* src, s21_size n);
extern char* s21_strchr(const char* str, int c);
extern int s21_strncmp(const char* str1, const char* str2, s21_size n);
extern char* s21_strncpy(char* dest, const char* src, s21_size n);
extern s21_size s21_strcspn(const char* str1, const char* str2);
extern char* s21_strerror(int errnum);
extern s21_size s21_strlen(const char* str);
extern char* s21_strpbrk(const char* str1, const char* str2);
extern char* s21_strrchr(const char* str, int c);
extern char* s21_strstr(const char* haystack, const char* needle);
extern char* s21_strtok(char* str, const char* delim);

/* Part 2-4: sprintf and sscanf */
extern int s21_sscanf(const char* str, const char* format, ...);
extern int s21_sprintf(char* str, const char* format, ...);

#endif
