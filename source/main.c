#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../header/data.h"
#include "../header/neurone.h"

#define SIZE_BUFFER 150


void pause() {system("pause");}

int main(int argc, char const *argv[])
{
	int col = 12;
	int ligne = 4900;
	char* csv = "../data/qualite-vin-blanc.csv";
	// Get data
	Data donnee = data_Load(col,ligne,csv,SIZE_BUFFER);

	// Normalized data
	if (donnee!=NULL) data_Normalized(donnee);
	else fprintf(stdout, "Erreur : Chargement des donnees");


	data_Free(donnee);
	pause();
}