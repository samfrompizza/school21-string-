# === PROJECT VARIABLES ====
CC = gcc
CFLAGS = -Wall -Werror -Wextra -O2 -g --std=gnu11
SANITIZE_FLAGS = -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=unreachable

ifdef SAN
CFLAGS += $(SANITIZE_FLAGS)
endif

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# sources
STRING_SRCS := $(wildcard src/string/**/*.c)
SSCANF_SRCS := $(wildcard src/sscanf/**/*.c)
SPRINTF_SRCS := $(wildcard src/sprintf/**/*.c)
EXTRA_SRCS := $(wildcard src/extra/**/*.c)
COMMON_SRCS := $(wildcard src/common/**/*.c)
SRCS := $(COMMON_SRCS) $(STRING_SRCS) $(SSCANF_SRCS) $(SPRINTF_SRCS) $(EXTRA_SRCS)

# objects
STRING_OBJS := $(patsubst src/%.c, build/%.o, $(STRING_SRCS))
SSCANF_OBJS := $(patsubst src/%.c, build/%.o, $(SSCANF_SRCS))
SPRINTF_OBJS := $(patsubst src/%.c, build/%.o, $(SPRINTF_SRCS))
EXTRA_OBJS := $(patsubst src/%.c, build/%.o, $(EXTRA_SRCS))
COMMON_OBJS := $(patsubst src/%.c, build/%.o, $(COMMON_SRCS))
OBJS := $(COMMON_OBJS) $(STRING_OBJS) $(SSCANF_OBJS) $(SPRINTF_OBJS) $(EXTRA_OBJS)

STRING_LIB = s21_string.a

# ==== TEST SOURCES ====
TEST_STRING_SRCS := $(wildcard tests/string/**/*.c)
TEST_SSCANF_SRCS := $(wildcard tests/sscanf/**/*.c)
TEST_SPRINTF_SRCS := $(wildcard tests/sprintf/**/*.c)
TEST_EXTRA_SRCS := $(wildcard tests/extra/**/*.c)
TEST_COMMON_SRCS := $(wildcard tests/common/**/*.c)

TEST_STRING_OBJS := $(patsubst tests/%.c, build/%.o, $(TEST_STRING_SRCS))
TEST_SSCANF_OBJS := $(patsubst tests/%.c, build/%.o, $(TEST_SSCANF_SRCS))
TEST_SPRINTF_OBJS := $(patsubst tests/%.c, build/%.o, $(TEST_SPRINTF_SRCS))
TEST_EXTRA_OBJS := $(patsubst tests/%.c, build/%.o, $(TEST_EXTRA_SRCS))
TEST_COMMON_OBJS := $(patsubst tests/%.c, build/%.o, $(TEST_COMMON_SRCS))

# ==== TOOLS ====
FORMATER = clang-format
LEAK_CHECKER = valgrind
LEAK_CHECKER_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
	TEST_LIBS = -lcheck -lm -lpthread -lrt -lsubunit
endif
ifeq ($(UNAME),Darwin)
	TEST_LIBS = -lcheck
endif

GCOV_FLAGS = -fprofile-arcs -ftest-coverage

# ==== TARGETS ====
.PHONY: all clean test rebuild gcov_report style format \
        leak_check leak_check_all leak_check_string leak_check_sscanf leak_check_sprintf leak_check_extra leak_check_common

all: $(STRING_LIB)

$(STRING_LIB): $(OBJS)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: tests/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ==== TEST TARGETS ====
test_string: build/tests/string/test_string.o $(TEST_COMMON_OBJS) $(STRING_LIB)
	$(CC) $(CFLAGS) $^ -o test_string $(TEST_LIBS)
	./test_string

test_sscanf: build/tests/sscanf/test_sscanf.o $(TEST_COMMON_OBJS) $(STRING_LIB)
	$(CC) $(CFLAGS) $^ -o test_sscanf $(TEST_LIBS)
	./test_sscanf

test_sprintf: build/tests/sprintf/test_sprintf.o $(TEST_COMMON_OBJS) $(STRING_LIB)
	$(CC) $(CFLAGS) $^ -o test_sprintf $(TEST_LIBS)
	./test_sprintf

test_extra: build/tests/extra/test_extra.o $(TEST_COMMON_OBJS) $(STRING_LIB)
	$(CC) $(CFLAGS) $^ -o test_extra $(TEST_LIBS)
	./test_extra

test_common: $(TEST_COMMON_OBJS) $(STRING_LIB)
	$(CC) $(CFLAGS) $^ -o test_common $(TEST_LIBS)
	./test_common

test_all: test_string test_sscanf test_sprintf test_extra test_common

test: test_all

# ==== LEAK CHECKS ====
leak_check_string: test_string
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./test_string || true

leak_check_sscanf: test_sscanf
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./test_sscanf || true

leak_check_sprintf: test_sprintf
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./test_sprintf || true

leak_check_extra: test_extra
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./test_extra || true

leak_check_common: test_common
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./test_common || true

leak_check_all: leak_check_string leak_check_sscanf leak_check_sprintf leak_check_extra leak_check_common

leak_check: leak_check_all

# ==== CLEAN / REBUILD ====
rebuild: clean all

clean:
	rm -rf $(BUILD_DIR) $(STRING_LIB) test_string test_sscanf test_sprintf test_extra test_common *.gc* report gcov_report.info

# ==== TOOLS ====
gcov_report:
	$(MAKE) clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRCS) $(TEST_STRING_SRCS) $(TEST_SSCANF_SRCS) $(TEST_SPRINTF_SRCS) $(TEST_EXTRA_SRCS) $(TEST_COMMON_SRCS) -o test_all $(TEST_LIBS)
	./test_all
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	open report/index.html
	$(MAKE) clean

style:
	$(FORMATER) -n $(shell find $(SRC_DIR) $(TEST_DIR) -name '*.c' -o -name '*.h')

format:
	$(FORMATER) -i $(shell find $(SRC_DIR) $(TEST_DIR) -name '*.c' -o -name '*.h')
