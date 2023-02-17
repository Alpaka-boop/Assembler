CC = g++

TARGET = assembler

ALL = main.cpp tree.cpp readwrite.cpp assembler.cpp

all: $(ALL)
	$(CC) $(ALL) -Wall -Wextra -o $(TARGET) 
