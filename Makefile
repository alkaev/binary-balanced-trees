CC = g++
CFLAGS=-Wall -Wextra -Werror -Iinclude -std=c++17 -g #-O3

.PHONY: all clean

all: mkDirObj binary-balanced-trees

mkDirObj:
		mkdir -p obj

clean:
	rm -rf obj binary-balanced-trees

binary-balanced-trees: obj/Generate.o obj/Cartesian_tree.o obj/Randomized_Cartesian_tree.o obj/RB_tree.o obj/main.o
		$(CC) $(CFLAGS) obj/Generate.o obj/Cartesian_tree.o obj/Randomized_Cartesian_tree.o obj/RB_tree.o obj/main.o -o binary-balanced-trees

obj/Generate.o obj/Generate.h: src/Generate.cpp
		$(CC) $(CFLAGS) -c src/Generate.cpp -o obj/Generate.o

obj/Cartesian_tree.o obj/Cartesian_tree.h: src/Cartesian_tree.cpp
		$(CC) $(CFLAGS) -c src/Cartesian_tree.cpp -o obj/Cartesian_tree.o

obj/Randomized_Cartesian_tree.o obj/Randomized_Cartesian_tree.h: src/Randomized_Cartesian_tree.cpp
		$(CC) $(CFLAGS) -c src/Randomized_Cartesian_tree.cpp -o obj/Randomized_Cartesian_tree.o

obj/RB_tree.o obj/RB_tree.h: src/RB_tree.cpp
		$(CC) $(CFLAGS) -c src/RB_tree.cpp -o obj/RB_tree.o

obj/main.o: src/main.cpp
		$(CC) $(CFLAGS) -c src/main.cpp -o obj/main.o
