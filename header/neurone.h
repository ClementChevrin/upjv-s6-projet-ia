// struct neurone
typedef struct neurone
{
	double *poids;
	double in,out,err;
}*Neurone;

// Fonction sigmoide
double neurone_Sigmoide(double x,double lambda);

// Init neurone struct
Neurone** neurone_Init(int nb_neurone, int couche);

// Free neurone struct
void neurone_Free(Neurone** n,int nb_neurone,int couche);

// Fonction d'apprentisage
double neurone_Apprentisage(Data donnee,int ligne,Neurone** n,double lambda);

// Fonction erreur
void neurone_Erreur(Data donnee,Neurone** n,double lambda);

// Fonction de correction des poids
void neurone_Correction(Data donnee,Neurone** n,double alpha,int ligne);

//Fonction de prédiction
int neurone_Prediction(Data donnee,int ligne,Neurone** n,double lambda);