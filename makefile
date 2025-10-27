# === Compiler configuration ===
CC = gcc
CFLAGS = -Wall -Wextra -g -O0 -I./src

# === Project structure ===
SRC_DIR = src
BUILD_DIR = build
TARGET = main

# === Files ===
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJ += $(BUILD_DIR)/main.o  # include object for main.c

# === Default rule ===
all: $(TARGET)

# === Link all objects into the final binary ===
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

# === Compile .c from src/ into .o in build/ ===
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# === Compile main.c separately ===
$(BUILD_DIR)/main.o: main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# === Ensure build directory exists ===
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# === Clean rule ===
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean

