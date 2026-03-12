#include "../../tests_includes/s21_tests.h"

START_TEST(insert_begin) {
  char src[] = "world";
  char ins[] = "hello ";
  char *res = (char *)s21_insert(src, ins, 0);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "hello world");
  free(res);
}
END_TEST

START_TEST(insert_middle) {
  char src[] = "hello!";
  char ins[] = " dear";
  char *res = (char *)s21_insert(src, ins, 5);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "hello dear!");
  free(res);
}
END_TEST

START_TEST(insert_end) {
  char src[] = "abc";
  char ins[] = "XYZ";
  char *res = (char *)s21_insert(src, ins, 3);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "abcXYZ");
  free(res);
}
END_TEST

START_TEST(insert_into_empty_src) {
  char src[] = "";
  char ins[] = "data";
  char *res = (char *)s21_insert(src, ins, 0);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "data");
  free(res);
}
END_TEST

START_TEST(insert_empty_str) {
  char src[] = "keep";
  char ins[] = "";
  char *res = (char *)s21_insert(src, ins, 2);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "keep");
  free(res);
}
END_TEST

START_TEST(start_index_too_large) {
  char src[] = "short";
  char ins[] = "X";
  void *res = s21_insert(src, ins, 6);
  ck_assert_ptr_eq(res, S21_NULL);
}
END_TEST

START_TEST(null_inputs) {
  void *r1 = s21_insert(S21_NULL, "a", 0);
  void *r2 = s21_insert("a", S21_NULL, 0);
  ck_assert_ptr_eq(r1, S21_NULL);
  ck_assert_ptr_eq(r2, S21_NULL);
}
END_TEST

Suite* suite_insert(void) {
  Suite* s = suite_create("suite_insert");
  TCase* tc = tcase_create("insert_tc");

  tcase_add_test(tc, insert_begin);
  tcase_add_test(tc, insert_middle);
  tcase_add_test(tc, insert_end);
  tcase_add_test(tc, insert_into_empty_src);
  tcase_add_test(tc, insert_empty_str);
  tcase_add_test(tc, start_index_too_large);
  tcase_add_test(tc, null_inputs);

  suite_add_tcase(s, tc);
  return s;
}