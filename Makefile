SRC_DIR := src
INCLUDE_DIR := src/include
OUT_FILE := CHIP-8.out

CFILES := $(shell find $(SRC_DIR) -type f -name "*.c")
W_FLAGS := -std=c99 -Wpedantic -pedantic-errors -Werror -Wall -Wextra \
	   -Waggregate-return -Wbad-function-cast -Wcast-align -Wcast-qual \
	   -Wfloat-equal -Wformat=2 -Wlogical-op \
	   -Wmissing-declarations -Wmissing-include-dirs -Wmissing-prototypes \
	   -Wnested-externs -Wpointer-arith -Wredundant-decls -Wsequence-point -Wshadow \
	   -Wstrict-prototypes -Wswitch -Wundef -Wunreachable-code \
	   -Wunused-but-set-parameter -Wwrite-strings
CFLAGS := -I '$(INCLUDE_DIR)' $(W_FLAGS)
SDL_FLAGS := -lSDL2
TEST_FLAGS := -DTEST $(CFLAGS) 

all: compile

clean:
	@ rm $(OUT_FILE)

compile:
	@ gcc $(CFLAGS) $(CFILES) $(SDL_FLAGS) -o $(OUT_FILE)

test:
	@ gcc $(TEST_FLAGS) $(CFILES) $(SDL_FLAGS) -o $(OUT_FILE)
