all:
	gcc -Wall -Wextra main.c -o sxbsbamdws

install: all
	cp sxbsbamdws /bin/
