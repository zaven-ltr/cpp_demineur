#include <iostream>
#include <stdlib.h>
#include "all_fct.h"

using namespace std;

//  LAUTIER Enzo

/* EXPLICATION RAPIDE :*/
/*
- mode de difficulter           ok
- point de vie                  ok
- differents degats (mine)      ok
- 3 joker                       ok
- score                         ok

// mode difficulter //

-plus le mode est facile 
        --> moins il y a de bombe
        --> plus on a de point de vie



// score //

-prend en compte differents facteur
        --> nombre de cases
        --> nombre de mines
        --> difficulter utiliser
        --> point de vie perdu
        --> nombre de joker utiliser


// degats mines //

-les degats par mines sont aléatoire 
        --> varie entre 1 et 3 

*/


/* !!*/
/* 
    // a certain moment du programme j'utilise le code ci-dessous
    cout<<system("cls")<<endl;
*/

int main()
{   
    /* CREATION DES ELEMENTS ESSENTIEL DU JEUX */
    Pre_Set p;                      //Structure d'éléments
    matrice* all_matrice[6];        //Tableaux de 5 pointeurs vers des Matrices

    /* INITIALISATION DU JEUX ET LANCEMENT EN BOUCLE DU JEUX */
    Init_game(p, all_matrice);      //Initialisation
    cout<<system("cls")<<endl;      //Clear
    boucle_game(p, all_matrice);    //Lancement en boucle

    return 0;
}
