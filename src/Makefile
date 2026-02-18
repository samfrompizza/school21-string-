CC = gcc
CFLAGS = -Wall -Werror -Wextra -O2 -g --std=gnu11

SRC_SPRINTF = $(shell find sprintf common -name '*.c')
OBJ_SPRINTF = $(patsubst %.c,build/%.o,$(SRC_SPRINTF))
TEST_SPRINTF = ../tests/sprintf/test_sprintf.c
TEST_OBJ = build/tests/sprintf/test_sprintf.o

LIB = s21_string.a

.PHONY: all clean test test_sprintf

all: $(LIB)

$(LIB): $(OBJ_SPRINTF)
	ar rcs $@ $^

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ): $(TEST_SPRINTF)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test_sprintf: $(LIB) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) $(LIB) -o test_sprintf
	./test_sprintf

test: test_sprintf

clean:
	rm -rf build $(LIB) test_sprintf
