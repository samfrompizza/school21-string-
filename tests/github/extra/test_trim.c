#include "../../tests_includes/s21_tests.h"

START_TEST(trim_default_whitespace) {
  char src[] = "  \t\n Hello Trim \r\n ";
  char *res = (char *)s21_trim(src, S21_NULL);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "Hello Trim");
  free(res);
}
END_TEST

START_TEST(trim_custom_chars) {
  char src[] = "xx--data--xx";
  char *res = (char *)s21_trim(src, "x-");
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "data");
  free(res);
}
END_TEST

START_TEST(trim_all_chars_result_empty) {
  char src[] = "xxxxx";
  char *res = (char *)s21_trim(src, "x");
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "");
  free(res);
}
END_TEST

START_TEST(trim_empty_src) {
  char src[] = "";
  char *res = (char *)s21_trim(src, S21_NULL);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "");
  free(res);
}
END_TEST

START_TEST(trim_no_trim_chars_match) {
  char src[] = "abc";
  char *res = (char *)s21_trim(src, "x");
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "abc");
  free(res);
}
END_TEST

START_TEST(null_input) {
  void *res = s21_trim(S21_NULL, " ");
  ck_assert_ptr_eq(res, S21_NULL);
}
END_TEST

Suite *suite_trim(void) {
  Suite *s = suite_create("suite_trim");
  TCase *tc = tcase_create("trim_tc");

  tcase_add_test(tc, trim_default_whitespace);
  tcase_add_test(tc, trim_custom_chars);
  tcase_add_test(tc, trim_all_chars_result_empty);
  tcase_add_test(tc, trim_empty_src);
  tcase_add_test(tc, trim_no_trim_chars_match);
  tcase_add_test(tc, null_input);

  suite_add_tcase(s, tc);
  return s;
}