execute.exe: main.c functions.c instruction_set.c functions.h instruction_set.h registers.h
	gcc main.c functions.c instruction_set.c -o execute.exe