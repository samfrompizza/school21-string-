CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
  TEST_LIBS = -lcheck -lm -lpthread -lrt -lsubunit
else
  TEST_LIBS = -lcheck -lm
endif

BUILD_DIR = build
LIB_NAME = s21_string.a

COMMON_SRCS = common/s21_format.c
SPRINTF_SRCS = sprintf/s21_sprintf.c sprintf/s21_sprintf_helpers.c
SRCS = $(COMMON_SRCS) $(SPRINTF_SRCS)
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

TEST_SPRINTF_SRC = ../tests/sprintf/test_sprintf.c
TEST_SPRINTF_OBJ = $(BUILD_DIR)/tests/sprintf/test_sprintf.o

.PHONY: all clean test gcov_report s21_string.a

all: s21_string.a

s21_string.a: $(OBJS)
	ar rcs $(LIB_NAME) $(OBJS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_SPRINTF_OBJ): $(TEST_SPRINTF_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: s21_string.a $(TEST_SPRINTF_OBJ)
	$(CC) $(CFLAGS) $(TEST_SPRINTF_OBJ) $(LIB_NAME) -o test_sprintf $(TEST_LIBS)
	./test_sprintf

gcov_report: CFLAGS += $(GCOV_FLAGS)
gcov_report: clean s21_string.a $(TEST_SPRINTF_OBJ)
	$(CC) $(CFLAGS) $(TEST_SPRINTF_OBJ) $(LIB_NAME) -o test_sprintf_gcov $(TEST_LIBS)
	./test_sprintf_gcov
	lcov -t "s21_sprintf" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info

clean:
	rm -rf $(BUILD_DIR) $(LIB_NAME) test_sprintf test_sprintf_gcov *.gcda *.gcno gcov_report.info report
