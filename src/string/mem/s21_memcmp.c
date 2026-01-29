#include "../s21_string.h"

int s21_memcmp(const void* str1, const void* str2, s21_size n) {
  const unsigned char* s1 = (const unsigned char*)str1;
  const unsigned char* s2 = (const unsigned char*)str2;
  int diff = 0;

  for (s21_size i = 0; i < n && diff == 0; i++) {
    if (s1[i] != s2[i]) {
      diff = s1[i] - s2[i];  // тут unsigned char же будет?
      // может не поместиться в int ?
    }
  }

  return diff;
}