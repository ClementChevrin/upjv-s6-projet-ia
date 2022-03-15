prog:main.o neurone.o data.o
	gcc main.o neurone.o data.o -o prog
main.o:main.c neurone.h data.h
	gcc -c main.c
data.o:data.c data.h
	gcc -c data.c
neurone.o:neurone.c neurone.h
	gcc -c neurone.c