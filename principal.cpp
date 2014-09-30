#include "maBiblio.h"

bool ValidationLigne(string, int);
int OptimiseLongueur(string);
vector<string> GrilleLettre(string, int);
vector<string> GrilleJeu(string, int);
void PromotionEspaceVide(vector<string>&);
void EspaceSort(vector<string>&, int, int, int);
int Partition(vector<string>&, int, int, int, int);
void EcrireFichier(vector<string>, vector<string>, int);
void AfficherJeu(vector<string>, vector<string>);

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "");
	ifstream ficIn;
	string nomFichier;
	string ligneCourante;
	int nbLigne = 0;
	int nbrColonnes = 0;
	int nbGenerer = 0;
	//string cadreHaut = "╔═";
	//string cadreCote = "║";
	//string cadreBas = "╚";

	do
	{
		cout << "Veuillez entrer le nom du fichier contenant les citations : ";

		cin >> nomFichier;
		ficIn.open(nomFichier);

		//Un peu comme Assert, si le fichie n'ouvre pas il affiche un message d'erreur.
	} while (!ficIn.is_open() && (cout << "Erreur de lecture.\n"));

	cout << "Fichier :" << nomFichier << " est en mode lecture.\n";


	while (ficIn.good())
	{
		nbLigne++;
		getline(ficIn, ligneCourante);
		/*
		int pos = ligneCourante.find_first_not_of("\n ");
		//find_first_not_of peut retourner string::npos (constante évalué à -1) si rien n'a été trouvé.
		//Si c'est le cas, ligne vide ou mal formaté, break à l'extérieur du for pour prendre la prochaine ligne.
		if (pos == string::npos)
		{
		cout << "Erreur ligne " << nbLigne << " citation vide.\n";
		continue;
		}

		ligneCourante = ligneCourante.substr(pos);
		*/

		//Si la ligne n'est pas valide, continue à la prochaine boucle.
		//À pour effet d'aller chercher la prochaine ligne.
		if (!ValidationLigne(ligneCourante, nbLigne))
			continue;

		//Le nombre de colonnes qui sera utilisé pour la grille de jeu de cette citation.
		nbrColonnes = OptimiseLongueur(ligneCourante);
		//Chaque string du vector est une ligne différente de la grille.
		//grilleLettre a les lettres de la grille supérieur.
		//grilleJeu a la représentation du jeu, l'emplacement où les lettres doivent aller.
		vector<string> grilleLettre = GrilleLettre(ligneCourante, nbrColonnes);
		vector<string> grilleJeu = GrilleJeu(ligneCourante, nbrColonnes);

		PromotionEspaceVide(grilleLettre);
		//Nombre de grille généré par le programme.
		nbGenerer++;
		AfficherJeu(grilleLettre, grilleJeu);
		EcrireFichier(grilleLettre, grilleJeu, nbGenerer);
		system("pause");

	}



	ficIn.close();
	cout << "Programme terminé\n";
	system("pause");

	//Si aucune citation a été généré, retourne le code 1.
	if (nbGenerer == 0)
		return 1;
	return 0;
}

/*
Fonction qui prend en paramètre deux vector<string> et qui affiche leur contenue.
Pour visualiser les grilles du jeu sur la console.
*/
void AfficherJeu(vector<string> haut, vector<string> bas)
{
	cout << endl << endl;

	for (string s : haut)
	{
		cout << s << endl;
	}

	cout << endl;

	for (string s : bas)
	{
		cout << s << endl;
	}

	cout << endl << endl;
}



/*
Fonction qui prend deux vector<string> et un int pour enregistrer le contenue des deux vector dans un fichier.
Pour enregistrer les grilles du jeu dans un fichier. Le nom du fichier est en fonction du int qui lui donne le nombre de la citation.
*/
void EcrireFichier(vector<string> sup, vector<string> inf, int nb)
{
	const string FORMAT = ".txt";
	ofstream ficOut;
	stringstream ss;
	//Utilisation de stringstream pour construire un objet string avec un string litteral et une variable int.
	ss << "Citation" << nb << FORMAT;

	ficOut.open(ss.str(), ios::out);
	//Enregistre ligne par ligne le contenue des vector dans le fichier.
	ficOut << endl;
	ficOut << endl;
	for (string s : sup)
	{
		ficOut << s;
		ficOut << endl;
	}
	for (string s : inf)
	{
		ficOut << endl;
		ficOut << s;
		ficOut << endl;
	}
	ficOut.close();

	cout << "Grille enregistrée à : " << ss.str() << endl;;
	//Vide le stringstream
	ss.str(string());
}

/*
Fonction qui prend un string et un int pour valider le string.
Le int est utilisé pour afficher un message d'erreur à l'utilisateur.
Valide une citation.
Retourne false si incorrect. True pour valide.
*/
bool ValidationLigne(string ligne, int nbr)
{
	const string SEPARATEUR = " -'.,";
	const int LONGUEUR_MINIMAL = 35;
	const int LONGUEUR_MAXIMAL = 100;
 	for (char c : ligne)
	{
		//Prend le code ascii du char
		int i = c;
		//Vérifie si le char est un charactère du ascii de base.
		if (i >= 128 || i < 0)
		{
			cout << "Erreur ligne " << nbr << " la citation contient des caractères illegaux.\n";
			system("pause");
			return false;
		}
	}

	if (ligne.length() < LONGUEUR_MINIMAL)
	{
		cout << "Erreur ligne " << nbr << " la citation contient moins que 35 caractères.\n";
		system("pause");
		return false;
	}

	if (ligne.length() > LONGUEUR_MAXIMAL)
	{
		cout << "Erreur ligne " << nbr << " la citation contient plus que 100 caractères.\n";
		system("pause");
		return false;
	}

	char precedent = ligne[0];
	for (int i = 1; i < ligne.length(); i++)
	{
		if ((SEPARATEUR.find(precedent) != string::npos ) && (SEPARATEUR.find(ligne[i]) != string::npos))
		{
			cout << "Erreur ligne " << nbr << " séparateur de mot consécutifs dans la citation.\n";
			system("pause");
			return false;
		}
		precedent = ligne[i];
	}


	return true;
}


/*
À partir d'un string, détermine le nombre de colonnes qui minimise les mots tronqués.
Retourne le int qui en résulte.
*/
int OptimiseLongueur(string ligne)
{
	const int MIN_CHAR_LIGNE = 13;
	const int MAX_CHAR_LIGNE = 17;
	int colonnes = 0;
	//Valeur arbitrairement grosse pour être presque certain (dans la majorité des cas d'utilisation)
	//qu'un meilleur résultat sera trouvé.
	int meilleurTronquage = 9999;

	//Vérifie selon la longueur minimale de 13 caractères par ligne jusqu'au maximum de 17 inclusivement.
	for (int i = MIN_CHAR_LIGNE; i <= MAX_CHAR_LIGNE; i++)
	{
		//Compteur de mots tronqués.
		int motTronquer = 0;
		//Le nombres de lignes. ceil est pour l'arrondir au plus grand entier supérieur.
		int times = ceil((double)ligne.length() / i);
		for (int t = 1; t <= times; t++)
		{
			/*
			Si la longueur de la ligne est plus petite ou égal que le nombre de caractère par ligne * le nombre de ligne, nous pouvons l'écrire sur une ligne au complet
			donc pas de mot tronqué.

			*/
			if ((ligne.length() > ((t * i))) && (ligne[t*i] != ' ') && (ligne[(t*i) - 1] != ' '))
			{
				motTronquer++;
			}
		}
		//Trouvé un meilleur résultat
		if (motTronquer < meilleurTronquage)
		{
			meilleurTronquage = motTronquer;
			colonnes = i;
		}
	}

	return colonnes;
}

/*
Créer la grille de lettre à partir d'un string et du nombre de colonnes.
Retourne le résultat sour la forme d'un vector<string> ou chaque string est une ligne différente.
*/
vector<string> GrilleLettre(string ligne, int colonne)
{
	const string SEPARATEUR = "-'.,";
	for (int i = 0; i < ligne.length(); i++)
	{
		if (SEPARATEUR.find(ligne[i]) != string::npos)
			ligne[i] = ' ';

	}
	vector<string> grilleLettre;

	//Retire des lettres de ligne au fure et à mesure pour les rajouter dans le vector grilleLettre.
	while (ligne.length() != 0)
	{
		//Retire la première ligne de la grille et la rajoute dans le vector.
		grilleLettre.push_back(ligne.substr(0, colonne));
		//Lorsqu'il ne reste pas assez de caractères à la string pour former une ligne au complète (selon le int colonne)
		//prend le restant de la string au complet. Pour éviter les erreurs d'exécution.
		ligne = ligne.substr(ligne.length() <= colonne ? ligne.length() : colonne);
	}
	//Rajoute des espacements vide à la dernière ligne de la grille pour avoir des string de taille uniforme.
	if (grilleLettre[grilleLettre.size() - 1].length() < colonne)
	{
		int i = colonne - grilleLettre[grilleLettre.size() - 1].length();
		string s(i, ' ');
		grilleLettre[grilleLettre.size() - 1] += s;
	}

	//Mélange les caractères des colonnes
	for (int i = 0; i < colonne; i++)
	{
		int nbrEchange = rand() % 50 + 1;
		for (int r = 0; r < nbrEchange; r++)
		{
			int premier = rand() % grilleLettre.size();;
			int deuxieme = rand() % grilleLettre.size();;
			//Fonction permuter disponible dans la librairie algorithm.
			swap(grilleLettre[premier][i], grilleLettre[deuxieme][i]);
		}
	}
	//Utilisation des itérateurs pour transformer chaque char en majuscule.
	for (int i = 0; i < grilleLettre.size(); i++)
	{
		transform(grilleLettre[i].begin(), grilleLettre[i].end(), grilleLettre[i].begin(), toupper);
	}
	return  grilleLettre;
}

/*
Créer la grille de jeu avec des emplacements vides ou des emplacements pour écrire des lettres du nombre de colonnes données.
Retourne un vector<string> qui est la grille formaté où chaque string est une ligne.
*/
vector<string> GrilleJeu(string ligne, int colonne)
{
	vector<string> grilleJeu;

	for (int i = 0; i < ligne.length(); i++)
	{
		//Si le caractère est alpha-numérique, on le remplace par un emplacement lettre.
		if (isalnum(ligne[i]))
		{
			ligne[i] = '-';
		}
		//Sinon, devient un emplacement vide.
		else
			ligne[i] = ' ';
	}

	//Rajoute les lignes dans la grilleJeu.
	while (ligne.length() != 0)
	{
		grilleJeu.push_back(ligne.substr(0, colonne));
		//Lorsqu'il ne reste pas assez de caractères à la string pour former une ligne au complète (selon le int colonne)
		//prend le restant de la string au complet. Pour éviter les erreurs d'exécution.
		ligne = ligne.substr(ligne.length() <= colonne ? ligne.length() : colonne);
	}


	return grilleJeu;
}


void PromotionEspaceVide(vector<string>& grille)
{
	//Pour chaque colonnes
	for (int i = 0; i < grille[0].length(); i++)
	{
		//Lance EspaceSort, envois le data, l'index du début, l'index de la fin et le numéro de la colonne traité.
		EspaceSort(grille, 0, grille.size() - 1, i);
	}

}



/*
Fonction pour créer un meilleur visuel.
Utilise un QuickSort implémenté pour travailler avec les colonnes d'un vector<string> dans le but de positionner les
emplacements vide au dessus des emplacements lettre.
Lors du mélange des caractères sur la colonnes, les emplacements vide pouvaient ce trouver n'importe où.
Puisque QuickSort est un algorithme instable, l'ordre des lettres n'est pas garanti.
*/
void EspaceSort(vector<string>& vecteur, int debut, int fin, int ligne)
{
	//Condition de sortie
	if (fin <= debut)
		return;
	//Index de la médianne déterminé au hasard pour débuté l'algorithme.
	int indexPivot = rand() % (fin - debut) + debut;
	//Index du nouveau pivot, relance l'algo de la partie gauche du pivot et ensuite de la partie de droite.
	indexPivot = Partition(vecteur, debut, fin, indexPivot, ligne);
	EspaceSort(vecteur, debut, indexPivot - 1, ligne);
	EspaceSort(vecteur, indexPivot + 1, fin, ligne);

}

/*
Partitionne une ligne du vecteur et déplace tout les caractères vide à gauche du pivot. 
*/
int Partition(vector<string>& vecteur, int debut, int fin, int pivot, int ligne)
{
	swap(vecteur[pivot][ligne], vecteur[fin][ligne]);

	int pivpost = debut;
	for (int i = debut; i < fin; i++)
	{
		if (vecteur[i][ligne] == ' ')
		{
			swap(vecteur[pivpost][ligne], vecteur[i][ligne]);
			pivpost += 1;
		}
	}
	swap(vecteur[pivpost][ligne], vecteur[fin][ligne]);
	return pivpost;
}