# Name of the executable
EXE_NAME = Nelly

# Directories
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = $(BIN_DIR)/obj

# Getting all source files (relative names)
CXX_SRC = $(wildcard $(SRC_DIR)/*.cxx)
CXX_SRC += $(wildcard $(SRC_DIR)/**/*.cxx)
CPP_SRC = $(wildcard $(SRC_DIR)/*.cpp)
CPP_SRC += $(wildcard $(SRC_DIR)/**/*.cpp)

# Generating object file names from source files (path/to/main.cxx -> main.o)
CXX_OBJ = $(CXX_SRC:.cxx=.o)
CPP_OBJ = $(CPP_SRC:.cpp=.o)
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(CXX_OBJ) $(CPP_OBJ)))

# Compiler options
CXX = g++
CXXFLAGS = -O3
CXXFLAGS += -Wall
CXXFLAGS += -Wextra

all: build

build: $(OBJ_FILES)
		$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$(EXE_NAME) $(OBJ_FILES)
		@echo "Build successful!"

$(OBJ_FILES): $(CXX_OBJ) $(CPP_OBJ)

$(CXX_OBJ): create_directories
		$(CXX) $(CXXFLAGS) -c $(@:.o=.cxx) -o $(OBJ_DIR)/$(notdir $@)

$(CPP_OBJ): create_directories
		$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp) -o $(OBJ_DIR)/$(notdir $@)

create_directories:
		@echo "Starting build..."
		@test -d $(OBJ_DIR) || mkdir -v -p $(OBJ_DIR)

clean:
		rm -rf $(OBJ_DIR) $(BIN_DIR)/$(EXE_NAME)

