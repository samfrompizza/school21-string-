#include "../s21_string.h"

int s21_memcmp(const void* str1, const void* str2, s21_size n) {
  const unsigned char* mem_block1 = (const unsigned char*)str1;
  const unsigned char* mem_block2 = (const unsigned char*)str2;
  int diff = 0;
  s21_size i = 0;

  for (; i < n && diff == 0; ++i) {
    if (mem_block1[i] != mem_block2[i]) {
      diff = (int)mem_block1[i] - (int)mem_block2[i];
    }
  }

  return diff;
}
