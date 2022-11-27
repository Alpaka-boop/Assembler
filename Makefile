CC = g++

TARGET = assembler

ALL = main.cpp tree.cpp output.cpp

all: $(ALL)
	$(CC) $(ALL) -Wall -Wextra -o $(TARGET)
