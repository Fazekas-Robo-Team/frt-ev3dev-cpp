BUILD_DIR := build
SRC_DIR := src
BIN_DIR := bin
TARGET := main
CXXFLAGS := -O3 --std=c++20 -Wall -Wextra -Wno-literal-suffix -Werror=infinite-recursion -Iinclude

include config.mk

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

OBJS := $(addsuffix .o,$(addprefix $(BUILD_DIR)/, $(notdir $(SRCS))))

$(BIN_DIR)/$(TARGET): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ 

$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp 
	mkdir -p $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) -o $@ $< 

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)/$(TARGET)
