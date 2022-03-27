typedef struct neurone
{
	double* poids;
	double entree, erreur;
}*Neurone;

// Fonction sigmoide
double neurone_Sigmoide(double x,double lambda);

void neurone_Erreur(Data d,Neurone** n,double lambda);

// Init neurone struct
Neurone** neurone_Init(int nb, int c);

// Free neurone struct
void neurone_Free(Neurone** n,int nb,int c);

// Fonction d'apprentisage
double neurone_Apprentisage(Data d,int ligne,Neurone** n,double lambda);

// Fonction de correction des poids
void neurone_Correction(Data d,Neurone** n,double alpha);