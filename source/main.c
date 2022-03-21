#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../header/data.h"
#include "../header/neurone.h"

#define SIZE_BUFFER 150

//void pause() {system("pause");}

int main(int argc, char const *argv[])
{
	int col = 12;
	int ligne = 4900;
	int nb_neurone = 12;
	int couche = 6;
	char* csv = "data/qualite-vin-blanc.csv";
	// Get data
	Data donnee = data_Load(col,ligne,csv,SIZE_BUFFER);

	// Normalized data
	if (donnee!=NULL) 
	{
		data_Normalized(donnee);

		// Apprentisage
		if (donnee!=NULL)
		{
			Neurone** neurone_array = neurone_Init(nb_neurone,couche);
			if (neurone_array != NULL)
			{

			 	neurone_Free(neurone_array,nb_neurone,couche);
			}	 
			else fprintf(stderr, "Erreur : Creation du tableau de neurone\n");
		}
		else fprintf(stderr, "Erreur : Normalisation des donnees\n");
		data_Free(donnee);
	}
	else fprintf(stderr, "Erreur : Chargement des donnees\n");

	
	//pause();
}