prog:binaire/main.o binaire/neurone.o binaire/data.o
	gcc binaire/main.o binaire/neurone.o binaire/data.o -o prog
binaire/main.o:source/main.c header/neurone.h header/data.h
	cd binaire && gcc -c ../source/main.c
binaire/data.o:source/data.c header/data.h
	cd binaire && gcc -c ../source/data.c
binaire/neurone.o:source/neurone.c header/neurone.h
	cd binaire && gcc -c ../source/neurone.c