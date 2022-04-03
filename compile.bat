cd binaire && gcc -c ../source/data.c
gcc -c ../source/neurone.c
gcc -c ../source/main.c
cd ../
gcc binaire/main.o binaire/data.o binaire/neurone.o prog
