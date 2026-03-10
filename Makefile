# =============================================================================
# s21_string+ Makefile
# =============================================================================

# -----------------------------------------------------------------------------
# Variables: Compiler & Flags
# -----------------------------------------------------------------------------
CC = gcc
CFLAGS = -Wall -Werror -Wextra -O2 -g --std=gnu11
SANITIZE_FLAGS = -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=unreachable
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

ifdef SAN
CFLAGS += $(SANITIZE_FLAGS)
endif

# -----------------------------------------------------------------------------
# Variables: Directories
# -----------------------------------------------------------------------------
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# -----------------------------------------------------------------------------
# Variables: Source Files (mirrors src/ hierarchy)
# -----------------------------------------------------------------------------
STRING_SRCS  := $(wildcard $(SRC_DIR)/string/*.c) $(wildcard $(SRC_DIR)/string/*/*.c)
SSCANF_SRCS  := $(wildcard $(SRC_DIR)/sscanf/*.c) $(wildcard $(SRC_DIR)/sscanf/*/*.c)
SPRINTF_SRCS := $(wildcard $(SRC_DIR)/sprintf/*.c) $(wildcard $(SRC_DIR)/sprintf/*/*.c)
EXTRA_SRCS   := $(wildcard $(SRC_DIR)/extra/*.c) $(wildcard $(SRC_DIR)/extra/*/*.c)
COMMON_SRCS  := $(wildcard $(SRC_DIR)/common/*.c) $(wildcard $(SRC_DIR)/common/*/*.c)
SRCS         := $(COMMON_SRCS) $(STRING_SRCS) $(SSCANF_SRCS) $(SPRINTF_SRCS) $(EXTRA_SRCS)

# -----------------------------------------------------------------------------
# Variables: Object Files
# -----------------------------------------------------------------------------
STRING_OBJS  := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(STRING_SRCS))
SSCANF_OBJS  := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SSCANF_SRCS))
SPRINTF_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SPRINTF_SRCS))
EXTRA_OBJS   := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(EXTRA_SRCS))
COMMON_OBJS  := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(COMMON_SRCS))
OBJS         := $(COMMON_OBJS) $(STRING_OBJS) $(SSCANF_OBJS) $(SPRINTF_OBJS) $(EXTRA_OBJS)

# -----------------------------------------------------------------------------
# Variables: Library
# -----------------------------------------------------------------------------
STRING_LIB = s21_string.a

# -----------------------------------------------------------------------------
# Variables: Local Project Tests (mirrors src/ under tests/)
# -----------------------------------------------------------------------------
TEST_STRING_SRCS  := $(wildcard $(TEST_DIR)/string/*.c) $(wildcard $(TEST_DIR)/string/*/*.c)
TEST_SSCANF_SRCS  := $(wildcard $(TEST_DIR)/sscanf/*.c) $(wildcard $(TEST_DIR)/sscanf/*/*.c)
TEST_SPRINTF_SRCS := $(wildcard $(TEST_DIR)/sprintf/*.c) $(wildcard $(TEST_DIR)/sprintf/*/*.c)
TEST_EXTRA_SRCS   := $(wildcard $(TEST_DIR)/extra/*.c) $(wildcard $(TEST_DIR)/extra/*/*.c)
TEST_COMMON_SRCS  := $(wildcard $(TEST_DIR)/common/*.c) $(wildcard $(TEST_DIR)/common/*/*.c)

TEST_STRING_OBJS  := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_STRING_SRCS))
TEST_SSCANF_OBJS  := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SSCANF_SRCS))
TEST_SPRINTF_OBJS := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SPRINTF_SRCS))
TEST_EXTRA_OBJS   := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_EXTRA_SRCS))
TEST_COMMON_OBJS  := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_COMMON_SRCS))

# -----------------------------------------------------------------------------
# Variables: GitHub Upstream Tests (tests/github/ hierarchy)
# -----------------------------------------------------------------------------
TEST_GITHUB_STRING_SUBDIRS := basic compare mem search strerror
TEST_GITHUB_SRCS := \
  $(wildcard $(TEST_DIR)/github/*.c) \
  $(wildcard $(TEST_DIR)/github/sscanf/*.c) \
  $(wildcard $(TEST_DIR)/github/sprintf/*.c) \
  $(foreach d,$(TEST_GITHUB_STRING_SUBDIRS),$(wildcard $(TEST_DIR)/github/string/$(d)/*.c))

TEST_GITHUB_OBJS := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_GITHUB_SRCS))

# -----------------------------------------------------------------------------
# Variables: Build Tools
# -----------------------------------------------------------------------------
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

# =============================================================================
# BUILD TARGETS
# =============================================================================

.PHONY: all
all: $(STRING_LIB)

$(STRING_LIB): $(OBJS)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Wno-unused-variable -Wno-stringop-truncation -c $< -o $@

# =============================================================================
# TEST TARGETS
# =============================================================================

.PHONY: test test_all test_scan test_sprintf test_github \
        test_github_string test_github_sscanf test_github_sprintf test_github_extra

# Run all tests (local + GitHub)
test: test_all
test_all: test_scan test_sprintf test_github

# Local: scan_string_var unit tests (sscanf core)
test_scan: $(BUILD_DIR)/sscanf/test_scan.o $(STRING_LIB)
	$(CC) $(CFLAGS) $(BUILD_DIR)/sscanf/test_scan.o $(STRING_LIB) -o test_scan $(TEST_LIBS)
	./test_scan

# Local: sprintf unit tests
test_sprintf: $(BUILD_DIR)/sprintf/test_sprintf.o $(STRING_LIB)
	$(CC) $(CFLAGS) $(BUILD_DIR)/sprintf/test_sprintf.o $(STRING_LIB) -o test_sprintf $(TEST_LIBS)
	./test_sprintf

# GitHub: full upstream test suite
test_github: $(STRING_LIB) $(TEST_GITHUB_OBJS)
	$(CC) $(CFLAGS) $(TEST_GITHUB_OBJS) $(STRING_LIB) -o test_github $(TEST_LIBS)
	./test_github

# GitHub: grouped by module (all use unified runner, run full suite per group)
test_github_string: test_github
test_github_sscanf: test_github
test_github_sprintf: test_github
test_github_extra: test_github

# =============================================================================
# LEAK CHECK TARGETS
# =============================================================================

.PHONY: leak_check leak_check_all leak_check_scan leak_check_sprintf leak_check_github

leak_check: leak_check_all
leak_check_all: leak_check_scan leak_check_sprintf leak_check_github

leak_check_scan: test_scan
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./test_scan || true

leak_check_sprintf: test_sprintf
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./test_sprintf || true

leak_check_github: test_github
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./test_github || true

# =============================================================================
# CLEAN & REBUILD
# =============================================================================

.PHONY: clean rebuild
rebuild: clean all

clean:
	rm -rf $(BUILD_DIR) $(STRING_LIB)
	rm -f test_scan test_sprintf test_github test_string test_sscanf test_extra test_common
	rm -f *.gc* report gcov_report.info

# =============================================================================
# TOOLS (style, format, gcov_report)
# =============================================================================

.PHONY: style format gcov_report
style:
	$(FORMATER) -n $(shell find $(SRC_DIR) $(TEST_DIR) -name '*.c' -o -name '*.h')

format:
	$(FORMATER) -i $(shell find $(SRC_DIR) $(TEST_DIR) -name '*.c' -o -name '*.h')

gcov_report:
	$(MAKE) clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRCS) $(TEST_STRING_SRCS) $(TEST_SSCANF_SRCS) \
	      $(TEST_SPRINTF_SRCS) $(TEST_EXTRA_SRCS) $(TEST_COMMON_SRCS) \
	      -o test_all $(TEST_LIBS)
	./test_all
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	open report/index.html
	$(MAKE) clean
