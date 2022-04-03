// struct data
typedef struct data
{
	int ligne;          // Nombre de ligne de donnee
	int colonne;        // Nombre de colonne de donnee
	double** critere;	// fixe_acidity           0
						// volatile_acidity       1
						// citric_acid            2
						// residual_sugar         3
						// chlorides              4
						// free_sulfur_dioxide    5
						// total_sulfur_dioxide   6
						// density                7
						// pH                     8
						// sulphates              9
						// alcohol                10
	                    // quality                11 
	double* min;        // Valeur minimale de chaque critere
	double* max;        // Valeur maximale de chaque critere
	double* coef;       // Coefficient de normalisation de chaque critere
}*Data;


// Init data struct
Data data_Init(int nb_donne,int ligne);

// Notify data struct
Data data_Notify(Data donnee,char* buffer,int ligne);

// Load data struct
Data data_Load(int colonne,int ligne,char* csv_path,int sizebuffer);

// Noramlized data struct
Data data_Normalized(Data donnee);

// Free data struct
void data_Free(Data donnee);