EXE_NAME = Nelly

SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = $(BIN_DIR)/obj

# Directories
CXX_SRC = $(wildcard $(SRC_DIR)/*.cxx)
CXX_SRC += $(wildcard $(SRC_DIR)/**/*.cxx)
CPP_SRC = $(wildcard $(SRC_DIR)/*.cpp)
CPP_SRC += $(wildcard $(SRC_DIR)/**/*.cpp)

CXX_OBJ = $(CXX_SRC:.cxx=.o)
CPP_OBJ = $(CPP_SRC:.cpp=.o)
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(CXX_OBJ) $(CPP_OBJ)))

# Compiler options
CXX = g++
CXXFLAGS = -Wall
# CXXFLAGS += -Wextra
CXXFLAGS += -O3

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
		@echo "Starting build!"
		@test -d $(OBJ_DIR) || mkdir -v $(OBJ_DIR)

clean:
		rm -rf $(OBJ_DIR) $(BIN_DIR)/$(EXE_NAME)

