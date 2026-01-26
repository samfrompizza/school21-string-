# === PROJECT VARIABLES ====
# compiler
CC = gcc
CFLAGS = -Wall -Werror -Wextra -O2 -g --std=gnu11
SANITIZE_FLAGS = -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=unreachable

# Use make SAN=1 to enable sanitizers
ifdef SAN
CFLAGS += $(SANITIZE_FLAGS)
endif

# directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# sources
STRING_SRCS := $(wildcard $(SRC_DIR)/string/**/*.c)
SSCANF_SRCS := $(wildcard $(SRC_DIR)/sscanf/**/*.c)
SPRINTF_SRCS := $(wildcard $(SRC_DIR)/sprintf/**/*.c)
EXTRA_SRCS := $(wildcard $(SRC_DIR)/extra/**/*.c)
COMMON_SRCS := $(wildcard $(SRC_DIR)/common/**/*.c)
SRCS := $(COMMON_SRCS) $(STRING_SRCS) $(SSCANF_SRCS) $(SPRINTF_SRCS) $(EXTRA_SRCS)

# objects
STRING_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(STRING_SRCS))
SSCANF_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SSCANF_SRCS))
SPRINTF_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SPRINTF_SRCS))
EXTRA_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(EXTRA_SRCS))
COMMON_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(COMMON_SRCS))
OBJS := $(COMMON_OBJS) $(STRING_OBJS) $(SSCANF_OBJS) $(SPRINTF_OBJS) $(EXTRA_OBJS)

# result
STRING_LIB = s21_string.a

# ==== TESTS VARIABLES ====
# sources
TEST_STRING_SRCS := $(wildcard $(TEST_DIR)/string/**/*.c)
TEST_SSCANF_SRCS := $(wildcard $(TEST_DIR)/sscanf/**/*.c)
TEST_SPRINTF_SRCS := $(wildcard $(TEST_DIR)/sprintf/**/*.c)
TEST_EXTRA_SRCS := $(wildcard $(TEST_DIR)/extra/**/*.c)
TEST_COMMON_SRCS := $(wildcard $(TEST_DIR)/common/**/*.c)

# objects
TEST_STRING_OBJS := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_STRING_SRCS))
TEST_SSCANF_OBJS := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SSCANF_SRCS))
TEST_SPRINTF_OBJS := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SPRINTF_SRCS))
TEST_EXTRA_OBJS := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_EXTRA_SRCS))
TEST_COMMON_OBJS := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_COMMON_SRCS))

# test executables
TEST_STRING_EXEC = test_string
TEST_SSCANF_EXEC = test_sscanf
TEST_SPRINTF_EXEC = test_sprintf
TEST_EXTRA_EXEC = test_extra
TEST_COMMON_EXEC = test_common
TEST_ALL_EXEC = test_all

# ==== TOOLS VARIABLES ====
# formatter
FORMATER = clang-format

# leak checker
LEAK_CHECKER = valgrind
LEAK_CHECKER_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

# platform-specific test libs
UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
	TEST_LIBS = -lcheck -lm -lpthread -lrt -lsubunit
endif
ifeq ($(UNAME),Darwin)
	TEST_LIBS = -lcheck
endif

# gcov flags
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

# ==== TARGETS ====
.PHONY: all clean test rebuild $(STRING_LIB) gcov_report style format \
        test_string test_sscanf test_sprintf test_extra test_common test_all \
        leak_check_string leak_check_sscanf leak_check_sprintf leak_check_extra leak_check_common leak_check_all

all: $(STRING_LIB)

$(STRING_LIB): $(OBJS)
	ar rcs $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ==== TESTS TARGETS ====
test_string: $(TEST_STRING_EXEC)
	./$(TEST_STRING_EXEC)

$(TEST_STRING_EXEC): $(TEST_STRING_OBJS) $(TEST_COMMON_OBJS) $(STRING_LIB)
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_LIBS)

test_sscanf: $(TEST_SSCANF_EXEC)
	./$(TEST_SSCANF_EXEC)

$(TEST_SSCANF_EXEC): $(TEST_SSCANF_OBJS) $(TEST_COMMON_OBJS) $(STRING_LIB)
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_LIBS)

test_sprintf: $(TEST_SPRINTF_EXEC)
	./$(TEST_SPRINTF_EXEC)

$(TEST_SPRINTF_EXEC): $(TEST_SPRINTF_OBJS) $(TEST_COMMON_OBJS) $(STRING_LIB)
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_LIBS)

test_extra: $(TEST_EXTRA_EXEC)
	./$(TEST_EXTRA_EXEC)

$(TEST_EXTRA_EXEC): $(TEST_EXTRA_OBJS) $(TEST_COMMON_OBJS) $(STRING_LIB)
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_LIBS)

test_common: $(TEST_COMMON_EXEC)
	./$(TEST_COMMON_EXEC)

$(TEST_COMMON_EXEC): $(TEST_COMMON_OBJS) $(STRING_LIB)
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_LIBS)

# ==== ALL TEST TARGETS ====
test_all: $(TEST_ALL_EXEC)
	./$(TEST_ALL_EXEC)

$(TEST_ALL_EXEC): $(TEST_STRING_OBJS) $(TEST_SSCANF_OBJS) $(TEST_SPRINTF_OBJS) $(TEST_EXTRA_OBJS) $(TEST_COMMON_OBJS) $(STRING_LIB)
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_LIBS)

test: test_all

# ==== LEAK CHECK TARGETS ====
leak_check_string: test_string
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./$(TEST_STRING_EXEC) || true

leak_check_sscanf: test_sscanf
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./$(TEST_SSCANF_EXEC) || true

leak_check_sprintf: test_sprintf
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./$(TEST_SPRINTF_EXEC) || true

leak_check_extra: test_extra
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./$(TEST_EXTRA_EXEC) || true

leak_check_common: test_common
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./$(TEST_COMMON_EXEC) || true

leak_check_all: test_all
	$(LEAK_CHECKER) $(LEAK_CHECKER_FLAGS) ./$(TEST_ALL_EXEC) || true

leak_check: leak_check_all

# ==== CLEAN AND REBUILD ====
rebuild: clean all

clean:
	rm -rf $(BUILD_DIR) $(STRING_LIB) $(TEST_STRING_EXEC) $(TEST_SSCANF_EXEC) $(TEST_SPRINTF_EXEC) $(TEST_EXTRA_EXEC) $(TEST_COMMON_EXEC) $(TEST_ALL_EXEC) *.gc* report gcov_report.info

# ==== TOOLS TARGETS ====
gcov_report:
	$(MAKE) clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRCS) $(TEST_STRING_SRCS) $(TEST_SSCANF_SRCS) $(TEST_SPRINTF_SRCS) $(TEST_EXTRA_SRCS) $(TEST_COMMON_SRCS) -o $(TEST_ALL_EXEC) $(TEST_LIBS)
	./$(TEST_ALL_EXEC)
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	open report/index.html
	$(MAKE) clean

style:
	$(FORMATER) -n $(shell find $(SRC_DIR) $(TEST_DIR) -name '*.c' -o -name '*.h')

format:
	$(FORMATER) -i $(shell find $(SRC_DIR) $(TEST_DIR) -name '*.c' -o -name '*.h')