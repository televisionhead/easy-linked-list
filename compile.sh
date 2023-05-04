#!/bin/sh
gcc main.c list.c -o main -static -Ofast -Wall -Wextra

#Use this one for compiling without the assert() statements
# gcc main.c list.c -o main -static -Ofast -Wall -Wextra -DNDEBUG