CC      := gcc
CFLAGS  := -std=c11 -O2 -Wall -Wextra -Iinclude -MMD -MP
LDLIBS  := $(shell sdl2-config --libs 2>/dev/null || echo -lSDL2)

SRC_DIR   := src
BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj

BIN := $(BUILD_DIR)/beni-boy

# Emulator core plus the SDL front end (main.c holds main).
SRC := $(addprefix $(SRC_DIR)/,cartridge.c cpu.c gb.c instructions.c \
       interrupts.c memory.c ppu.c timer.c main.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all
all: $(BIN)

$(BIN): $(OBJ) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

-include $(OBJ:.o=.d)
