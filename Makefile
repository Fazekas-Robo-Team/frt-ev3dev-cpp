BUILD_DIR := build
SRC_DIR := src
BIN_DIR := bin
EXECUTABLE := main
CXXFLAGS := -O3 --std=c++20

include config.mk

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

OBJS := $(addsuffix .o,$(addprefix $(BUILD_DIR)/, $(notdir $(SRCS))))

$(BIN_DIR)/$(EXECUTABLE): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ 

$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp 
	mkdir -p $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) -o $@ $< 

run: clean $(BUILD_DIR)/$(EXECUTABLE)
	$(BUILD_DIR)/$(EXECUTABLE)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)/$(EXECUTABLE)