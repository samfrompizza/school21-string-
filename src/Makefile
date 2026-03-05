CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
INCLUDES = -I. -Istring string/s21_string.h sprintf/s21_sprintf.h sprintf/s21_sprintf_helpers.h

UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
  TEST_LIBS = -lcheck -lm -lpthread -lrt -lsubunit
else
  TEST_LIBS = -lcheck -lm
endif

BUILD_DIR = ../build
LIB_NAME = $(BUILD_DIR)/s21_string.a

COMMON_SRCS = common/s21_format.c
SPRINTF_SRCS = sprintf/s21_sprintf.c sprintf/s21_sprintf_helpers.c

STRING_SRCS := $(wildcard string/*.c) $(wildcard string/*/*.c)

SRCS = $(COMMON_SRCS) $(SPRINTF_SRCS) $(STRING_SRCS)
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

TEST_SPRINTF_SRC = sprintf/test_sprintf.c
TEST_SPRINTF_OBJ = $(BUILD_DIR)/tests/sprintf/test_sprintf.o

.PHONY: all clean test gcov_report

all: $(LIB_NAME)

$(LIB_NAME): $(OBJS)
	@mkdir -p $(dir $@)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_SPRINTF_OBJ): $(TEST_SPRINTF_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: $(LIB_NAME) $(TEST_SPRINTF_OBJ)
	$(CC) $(CFLAGS) $(TEST_SPRINTF_OBJ) $(LIB_NAME) -o test_sprintf $(TEST_LIBS)
	./test_sprintf

clean:
	rm -rf $(BUILD_DIR) $(LIB_NAME) test_sprintf test_sprintf_gcov *.gcda *.gcno gcov_report.info report
