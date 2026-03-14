#include "../../tests_includes/s21_tests.h"

START_TEST(empty) {
  char src[] = "";
  char *res = (char *)s21_to_upper(src);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "");
  free(res);
}
END_TEST

START_TEST(mixed_ascii) {
  char src[] = "AbcDe123! z";
  char expect[] = "ABCDE123! Z";
  char *res = (char *)s21_to_upper(src);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, expect);
  free(res);
}
END_TEST

START_TEST(numbers_and_symbols) {
  char src[] = "12345!@#";
  char *res = (char *)s21_to_upper(src);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "12345!@#");
  free(res);
}
END_TEST

START_TEST(already_upper) {
  char src[] = "HELLO WORLD";
  char *res = (char *)s21_to_upper(src);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "HELLO WORLD");
  free(res);
}
END_TEST

START_TEST(null_input) {
  void *res = s21_to_upper(S21_NULL);
  ck_assert_ptr_eq(res, S21_NULL);
}
END_TEST

Suite *suite_to_upper(void) {
  Suite *s = suite_create("suite_to_upper");
  TCase *tc = tcase_create("to_upper_tc");

  tcase_add_test(tc, empty);
  tcase_add_test(tc, mixed_ascii);
  tcase_add_test(tc, numbers_and_symbols);
  tcase_add_test(tc, already_upper);
  tcase_add_test(tc, null_input);

  suite_add_tcase(s, tc);
  return s;
}