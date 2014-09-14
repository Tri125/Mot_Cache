#include "maBiblio.h"

bool ValidationLigne(string, int);

int main()
{
	setlocale(LC_ALL, "");
	ifstream ficIn;
	ofstream ficOut;
	string nomFichier;
	string ligneCourante;
	int nbLigne = 0;

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

		if (ValidationLigne(ligneCourante, nbLigne))
		{
			cout << endl;
		}
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