#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../header/data.h"
#include "../header/neurone.h"

#define SIZE_BUFFER 150

int main(int argc, char *argv[])
{
	double lambda = 1;
	double alpha = 0.2;
	double err_Accept=0.25;
	char* csv = (char*)malloc(sizeof(char)*27);
	strcpy(csv,"data/qualite-vin-blanc.csv");
	
	if (argc>1)
	{
		if (argc==5)
		{
			strcpy(csv,argv[1]);
			lambda = atof(argv[2]);
			alpha = atof(argv[3]);
			err_Accept = atof(argv[4]);
		}
		else
		{
			strcpy(csv,argv[1]);
			lambda = 1;
			alpha = 0.2;
			err_Accept = 0.20;
		}
	}
	int col = 0;
	int ligne = 1;
	int nb_neurone = 0;
	int couche = 0;
	FILE* calcul = fopen(csv,"r");
	if (calcul !=NULL)
	{
		char buffer[SIZE_BUFFER];
		while(fgets(buffer,SIZE_BUFFER,calcul)!=NULL)
		{
			if (ligne==1)
			{
				const char * separators = ",;";
			    char * strToken = strtok ( buffer, separators );
			    while ( strToken != NULL ) 
			    {    	
			    	col++;
			        strToken = strtok ( NULL, separators );
			    }
			}	
			ligne++;	
		}
		double err;
		int ligne_app=(ligne*90)/100;
		int ligne_predict=ligne-ligne_app;
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
				int nb_neuronebis = nb_neurone;
				while(nb_neuronebis)
				{
					couche++;
					nb_neuronebis /= 2;
				}
				Neurone** neurone_array = neurone_Init(nb_neurone,couche);
				if (neurone_array != NULL)
				{
					//Apprentissage sur les 90% du fichier
					for (int j = 0; j < ligne_app; ++j)
					{
						err=1;
						int cpt=6;
						while(err>err_Accept || err<-err_Accept)
						{
							err=neurone_Apprentisage(donnee,j,neurone_array,lambda);
							if(err>err_Accept || err<-err_Accept)
							{
								neurone_Erreur(donnee,neurone_array,lambda);
								neurone_Correction(donnee,neurone_array,alpha,j);
								if(j==1) cpt--;
								if(!cpt) break;
							}
						}
					}
					//Prédiction des 10% restants du fichier
					int correct=0;
					for(int j=(ligne-ligne_predict);j<ligne;++j) correct+=neurone_Prediction(donnee,j,neurone_array,lambda);
					printf("Nombre de predictions correctes = %d/%d\n",correct,ligne_predict);
				 	neurone_Free(neurone_array,nb_neurone,couche);
				}	 
				else fprintf(stderr, "Erreur : Creation du tableau de neurone\n");
			}
			else fprintf(stderr, "Erreur : Normalisation des donnees\n");
			data_Free(donnee);
		}
		else fprintf(stderr, "Erreur : Chargement des donnees\n");
	}
	else fprintf(stderr, "Erreur : Calcul colonne et ligne\n");
	if(csv!=NULL)
		free(csv);
}