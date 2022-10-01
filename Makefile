CC = g++

# The build target 
TARGET = Nelly

all: $(TARGET)

$(TARGET): cpp-logger/*.cpp chess/*.cpp chess/pieces/*.cpp *.cpp
	$(CC) -o $(TARGET) cpp-logger/*.cpp chess/*.cpp chess/pieces/*.cpp *.cpp
clean:
	$(RM) -r chess cpp-logger LICENSE *.cpp *.hpp README.md $(TARGET)
