CFLAGS = -std=c99 -pedantic -Wall -Wextra -DENABLE_ASSERT -I./moorhen/include
CFLAGS += -Wno-unused-variable
LDFLAGS = -lm -lpthread -ffast-math #-O3
LDFLAGS += -lxcb
LDFLAGS += -lvulkan -ldl -lX11 -lXxf86vm -lXrandr -lXi

SRC_DIR=src
BUILD_DIR=build

$(BUILD_DIR)/main: $(SRC_DIR)/main.c
	cd moorhen && make build
	gcc $(SRC_DIR)/*.c moorhen/build/libmoorhen.a -o $@  $(CFLAGS) $(LDFLAGS)
$(BUILD_DIR)/main_gdb: $(SRC_DIR)/main.c
	cd moorhen && make build
	gcc -ggdb $(SRC_DIR)/*.c moorhen/build/libmoorhen.a -o $@  $(CFLAGS) $(LDFLAGS)

.PHONY: clean test gdb

clean:
	rm -rf $(BUILD_DIR)/*

test: $(BUILD_DIR)/main
	./$(BUILD_DIR)/main

gdb: $(BUILD_DIR)/main_gdb
	gdb ./$(BUILD_DIR)/main_gdb corefile
