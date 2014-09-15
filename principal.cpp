#include "maBiblio.h"

bool ValidationLigne(string, int);
int OptimiseLongueur(string);
vector<string> GrilleLettre(string, int);
vector<string> GrilleJeu(string, int);

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "");
	ifstream ficIn;
	ofstream ficOut;
	string nomFichier;
	string ligneCourante;
	int nbLigne = 0;
	int nbrColonnes = 0;

	do
	{
		cout << "Veuillez entrer le nom du fichier contenant les citations : ";

		cin >> nomFichier;
		ficIn.open(nomFichier);

		//Un peu comme Assert, si le fichie n'ouvre pas il affiche un message d'erreur.
	} while (!ficIn.is_open() && (cout << "Erreur de lecture.\n"));

	cout << "Fichier :" << nomFichier << " est en mode lecture.";


	while (ficIn.good())
	{
		nbLigne++;
		getline(ficIn, ligneCourante);
		int pos = ligneCourante.find_first_not_of("\n ");
		//find_first_not_of peut retourner string::npos (constante évalué à -1) si rien n'a été trouvé.
		//Si c'est le cas, ligne vide ou mal formaté, break à l'extérieur du for pour prendre la prochaine ligne.
		if (pos == string::npos)
		{
			cout << "Erreur ligne " << nbLigne << " citation vide.\n";
			continue;
		}

		ligneCourante = ligneCourante.substr(pos);

		if (!ValidationLigne(ligneCourante, nbLigne))
			continue;
		nbrColonnes = OptimiseLongueur(ligneCourante);
		GrilleLettre(ligneCourante, nbrColonnes);
		GrilleJeu(ligneCourante, nbrColonnes);
		system("pause");
	}


	ficIn.close();
	system("pause");
	return 0;
}


bool ValidationLigne(string ligne, int nbr)
{
	for (char c : ligne)
	{
		int i = c;
		if (i >= 128 || i < 0)
		{
			cout << "Erreur ligne " << nbr << " la citation contient des caractères illegaux.\n";
			return false;
		}
	}

	if (ligne.length() < 35)
	{
		cout << "Erreur ligne " << nbr << " la citation contient moins que 35 caractères.\n";
		return false;
	}

	if (ligne.length() > 100)
	{
		cout << "Erreur ligne " << nbr << " la citation contient plus que 100 caractères.\n";
		return false;
	}

	int pos = ligne.find("  ");

	if (pos != string::npos)
	{
		cout << "Erreur ligne " << nbr << " séparateur de mot consécutifs dans la citation.\n";
		return false;
	}


	return true;
}

int OptimiseLongueur(string ligne)
{
	int colonnes = 0;
	int meilleurTronquage = 9999;

	for (int i = 13; i < 18; i++)
	{
		int motTronquer = 0;
		int times = ceil((double)ligne.length() /i);
		for (int t = 1; t <= times; t++)
		{
			if ((ligne.length() > ((t * i))) && (ligne[t*i] != ' ') && (ligne[(t*i) -1] != ' '))
			{
				motTronquer++;
			}
		}
		if (motTronquer < meilleurTronquage)
		{
			meilleurTronquage = motTronquer;
			colonnes = i;
		}
	}

	return colonnes;
}


vector<string> GrilleLettre(string ligne, int colonne)
{
	vector<string> grilleLettre;


	while (ligne.length() != 0)
	{
		grilleLettre.push_back(ligne.substr(0, colonne));
		ligne = ligne.substr(ligne.length() <= colonne ? ligne.length() : colonne);
	}
	if (grilleLettre[grilleLettre.size() - 1].length() < colonne)
	{
		int i = colonne - grilleLettre[grilleLettre.size() - 1].length();
		string s(i, ' ');
		grilleLettre[grilleLettre.size() - 1] += s;
	}

	cout << "";

	for (int i = 0; i < colonne; i++)
	{
		int nbrEchange = rand() % 50 + 1;
		for (int r = 0; r < nbrEchange; r++)
		{
			int premier = rand() % grilleLettre.size();;
			int deuxieme = rand() % grilleLettre.size();;
	

			//do
			//{
			//	premier = rand() % grilleLettre.size();
			//	deuxieme = rand() % grilleLettre.size();
			//} while ((premier == grilleLettre.size()-1 && i > grilleLettre[premier].length()) || (deuxieme == grilleLettre.size()-1 && i > grilleLettre[deuxieme].length()));
			swap(grilleLettre[premier][i],grilleLettre[deuxieme][i]);
		}
	}
	cout << endl;
	for (string s : grilleLettre)
	{
		cout << s;
		cout << endl;
	}
	return  grilleLettre;
}


vector<string> GrilleJeu(string ligne, int colonne)
{
	vector<string> grilleJeu;

	for (int i = 0; i < ligne.length();i++)
	{
		if (isalnum(ligne[i]))
		{
			ligne[i] = '-';
		}
		else
			ligne[i] = ' ';
	}

	while (ligne.length() != 0)
	{
		grilleJeu.push_back(ligne.substr(0, colonne));
		ligne = ligne.substr(ligne.length() <= colonne ? ligne.length() : colonne);
	}


	cout << endl;
	for (string s : grilleJeu)
	{
		cout << s;
		cout << endl;
	}
	return grilleJeu;
}