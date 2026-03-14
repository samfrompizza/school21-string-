CC = gcc
AR = ar
RM = rm -rf

CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS =
FORMATER = clang-format
LEAK_CHECKER = valgrind
LEAK_CHECKER_FLAGS = --leak-check=full --show-leak-kinds=all --error-exitcode=1

SRC_DIR = .
TEST_DIR = ../tests
BUILD_DIR = ../build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
REPORT_DIR = ../report

LIB_NAME = s21_string.a
TEST_SCAN_BIN = $(BIN_DIR)/test_scan
TEST_SPRINTF_BIN = $(BIN_DIR)/test_sprintf
TEST_GITHUB_BIN = $(BIN_DIR)/test_github

LIB_SRCS = $(shell find $(SRC_DIR) -type f -name '*.c' | sort)

LIB_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(LIB_SRCS))

TEST_SCAN_SRC = $(TEST_DIR)/sscanf/test_scan.c
TEST_SPRINTF_SRC = $(TEST_DIR)/sprintf/test_sprintf.c
TEST_GITHUB_SRCS = $(filter-out $(TEST_DIR)/github/s21_tests.c, \
	$(shell find $(TEST_DIR)/github -type f -name '*.c' | sort))
TEST_GITHUB_MAIN = $(TEST_DIR)/github/s21_tests.c

UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
	CHECK_LIBS = -lcheck -lm -lpthread -lrt -lsubunit
else
	CHECK_LIBS = -lcheck -lm -lpthread
endif

ifeq ($(GCOV),1)
	CFLAGS += -fprofile-arcs -ftest-coverage -O0
	LDFLAGS += -fprofile-arcs -ftest-coverage
endif

.PHONY: all clean test gcov_report

all: $(LIB_NAME)

$(LIB_NAME): $(LIB_OBJS)
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_SCAN_BIN) $(TEST_SPRINTF_BIN) $(TEST_GITHUB_BIN)
	$(TEST_SCAN_BIN)
	$(TEST_SPRINTF_BIN)
	$(TEST_GITHUB_BIN)

$(TEST_SCAN_BIN): $(TEST_SCAN_SRC) $(LIB_NAME)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(LIB_NAME) -o $@ $(CHECK_LIBS) $(LDFLAGS)

$(TEST_SPRINTF_BIN): $(TEST_SPRINTF_SRC) $(LIB_NAME)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(LIB_NAME) -o $@ $(CHECK_LIBS) $(LDFLAGS)

$(TEST_GITHUB_BIN): $(TEST_GITHUB_MAIN) $(TEST_GITHUB_SRCS) $(LIB_NAME)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(TEST_GITHUB_MAIN) $(TEST_GITHUB_SRCS) \
		$(LIB_NAME) -o $@ $(CHECK_LIBS) $(LDFLAGS)

gcov_report: clean
	$(MAKE) GCOV=1 test
	lcov --capture --directory $(OBJ_DIR) --output-file $(BUILD_DIR)/coverage.info
	lcov --remove $(BUILD_DIR)/coverage.info '/usr/*' '$(TEST_DIR)/*' --output-file $(BUILD_DIR)/coverage.filtered.info
	genhtml $(BUILD_DIR)/coverage.filtered.info --output-directory $(REPORT_DIR)

style:
	$(FORMATER) -n $(shell find $(SRC_DIR) $(TEST_DIR) -name '*.c' -o -name '*.h')

format:
	$(FORMATER) -i $(shell find $(SRC_DIR) $(TEST_DIR) -name '*.c' -o -name '*.h')

leak_check: leak_check_all
leak_check_all: leak_check_scan leak_check_sprintf leak_check_github

leak_check_scan: $(TEST_SCAN_BIN)
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) $(BIN_DIR)/test_scan 2>&1 | grep "ERROR SUMMARY" || true

leak_check_sprintf: $(TEST_SPRINTF_BIN)
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) $(BIN_DIR)/test_sprintf 2>&1 | grep "ERROR SUMMARY" || true

leak_check_github: $(TEST_GITHUB_BIN)
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) $(BIN_DIR)/test_github 2>&1 | grep "ERROR SUMMARY" || true

clean:
	$(RM) $(BUILD_DIR) $(REPORT_DIR) $(LIB_NAME) *.gcno *.gcda *.info