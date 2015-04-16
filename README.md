# Mot_Cache

À partir d'un fichier contenant une citation par ligne, génère un fichier par citation composé de deux grilles de jeu.

```
Citation: Chacun admire le passe regrette le present et tremble pour l avenir 

Grille généré:

 E EO     A E  L 
LPSSENNTDETVTNLE 
PARPUERALMETEREME
CHACSUREGRIRTEIRB

------ ------ -- 

----- -------- --

 ------- -- -----

-- ---- - ------ 
```

* La grille inférieur contient des emplacements vides où les lettres de la citation doivent être placées.
* La grille est sur plusieurs lignes et généré de façon à avoir au minimum 13 caractères par ligne, au maximum 17 caractères par ligne
et à minimiser les mots tronqués.
* La grille supérieur contient les lettres de la colonne, mais mélangé au hasard.
* Chaque citation est validé pour s'assurer qu'aucun caractère ascii étendu est utilisé, que la longueur est entre 35 et 100 caractères et qu'il
n'y a pas deux séparateurs de mots consécutifs.
