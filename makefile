prog:main.o neurone.o data.o
	gcc binaire/main.o binaire/neurone.o binaire/data.o -o prog -lm
main.o:source/main.c header/neurone.h header/data.h
	cd binaire && gcc -c ../source/main.c
data.o:source/data.c header/data.h
	cd binaire && gcc -c ../source/data.c
neurone.o:source/neurone.c header/neurone.h
	cd binaire && gcc -c ../source/neurone.c 