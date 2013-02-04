all:
	clang \
		-o ant \
		-Wall -ansi -pedantic -ggdb \
		ant.c types.c utils.c \
		-lcurses
