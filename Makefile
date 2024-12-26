# Compiler and flags
CC = g++
CFLAGS = -Isrc/include
LDFLAGS = -Lsrc/lib -lmingw32 -lSDL2main -lSDL2

# Target executable
TARGET = Dijkstra_Algorithm

# Source files
SRC = main2.cpp

# Default rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Clean rule
clean:
	rm -f $(TARGET)
