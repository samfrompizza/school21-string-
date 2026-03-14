#include "../../tests_includes/s21_tests.h"

START_TEST(empty) {
  char src[] = "";
  char *res = (char *)s21_to_lower(src);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "");
  free(res);
}
END_TEST

START_TEST(mixed_ascii) {
  char src[] = "AbCDe123! Z";
  char expect[] = "abcde123! z";
  char *res = (char *)s21_to_lower(src);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, expect);
  free(res);
}
END_TEST

START_TEST(numbers_and_symbols) {
  char src[] = "12345!@#";
  char *res = (char *)s21_to_lower(src);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "12345!@#");
  free(res);
}
END_TEST

START_TEST(already_lower) {
  char src[] = "hello world";
  char *res = (char *)s21_to_lower(src);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "hello world");
  free(res);
}
END_TEST

START_TEST(null_input) {
  void *res = s21_to_lower(S21_NULL);
  ck_assert_ptr_eq(res, S21_NULL);
}
END_TEST

Suite *suite_to_lower(void) {
  Suite *s = suite_create("suite_to_lower");
  TCase *tc = tcase_create("to_lower_tc");

  tcase_add_test(tc, empty);
  tcase_add_test(tc, mixed_ascii);
  tcase_add_test(tc, numbers_and_symbols);
  tcase_add_test(tc, already_lower);
  tcase_add_test(tc, null_input);

  suite_add_tcase(s, tc);
  return s;
}