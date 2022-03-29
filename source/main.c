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
	int ligne_app=(ligne*90)/100;
	int ligne_predict=ligne-ligne_app;
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
					//Apprentissage sur les 90% du fichier
					for (int j = 0; j < ligne_app; ++j)
					{
						err=1;
						while(err>0.15 || err<-0.15)
						{
							err=neurone_Apprentisage(donnee,j,neurone_array,lambda);
							neurone_Erreur(donnee,neurone_array,lambda);
							neurone_Correction(donnee,neurone_array,alpha);
						}
					}
					//Prédiction des 10% restants du fichier
					/*printf("\n\n\n");
					int correct=0;
					for(int j=(ligne-ligne_predict);j<(ligne-ligne_predict)+3;++j)
					{
						correct+=neurone_Prediction(donnee,j,neurone_array,lambda);
					}
					printf("nombre de predictions correctes=%d\n",correct);*/
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