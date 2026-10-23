CXX ?= g++
CPPFLAGS ?= -Isrc -Isrc/cpp-logger/include
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
LDFLAGS ?=
LDLIBS ?= -pthread
BIN_DIR ?= bin
ENGINE_SRC = src/chess/board.cpp src/chess/pieces.cpp src/cpp-logger/src/logger.cpp
ENGINE_OBJ = $(patsubst %.cpp,$(BIN_DIR)/obj/%.o,$(ENGINE_SRC))
MAIN_OBJ = $(BIN_DIR)/obj/src/main.o
TEST_OBJ = $(BIN_DIR)/obj/tests/perft.o

all: $(BIN_DIR)/Nelly

$(BIN_DIR)/Nelly: $(MAIN_OBJ) $(ENGINE_OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BIN_DIR)/perft-tests: $(TEST_OBJ) $(ENGINE_OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Keep the logger quiet at startup while retaining engine assertions.
$(BIN_DIR)/obj/src/cpp-logger/src/logger.o: CPPFLAGS += -DNDEBUG

$(BIN_DIR)/obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

test: $(BIN_DIR)/Nelly $(BIN_DIR)/perft-tests
	$(BIN_DIR)/perft-tests
	python3 tests/cli.py $(BIN_DIR)/Nelly

clean:
	$(RM) -r $(BIN_DIR)

-include $(MAIN_OBJ:.o=.d) $(TEST_OBJ:.o=.d) $(ENGINE_OBJ:.o=.d)
.PHONY: all test clean
