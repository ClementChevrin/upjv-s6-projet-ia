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
	int nb_neurone = col;
	int couche = 4;
	double lambda = 1;
	double alpha = 0.3;
	double err;


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
			nb_neurone = (col-1>donnee->max[col-1]-donnee->min[col-1]) ? col-1 : donnee->max[col-1]-donnee->min[col-1];
			Neurone** neurone_array = neurone_Init(nb_neurone,couche);
			if (neurone_array != NULL)
			{
				for (int i = 0; i < 1; ++i)
				{
					for (int j = 0; j < 100; ++j)
					{
						printf("\n\nLigne %d\n\n",j);
						int cpt=4;
						err=1;
						while(err>0.15 || err<-0.15)
						{
							//printf("\ntour\n");
							err=neurone_Apprentisage(donnee,i,neurone_array,lambda);
							neurone_Erreur(donnee,neurone_array,lambda);
							neurone_Correction(donnee,neurone_array,alpha);
							//cpt--;
						}
					}
				}
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