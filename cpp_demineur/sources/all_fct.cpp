#include <iostream>
#include "all_fct.h"

using namespace std;

/* GROUPE CREATION MATRICES */

//  2- appellé lors de la creation d'une matrice elle retourne une grille
int** Creer_tab2D(int n, int m)
{
    //pour que la grille est une epaisseur
    //l'utilité de cela seras exploiter plus tard
    n = n + 2;
    m = m + 2;

    int** p = new int*[n];

    for (int i = 0; i < n; i++)
    {
        p[i] = new int[m];
    }
    
    return(p);
}

//  4- Initialise toute nouvelle matrice a 0
void initZero_mat(matrice &J)
{
    for (int i = 0; i < J.nbl; i++)
    {
        for (int j = 0; j < J.nbc; j++)
        {
            J.T[i][j] = 0;
        }
    }
}

//  2- Retourne un pointeur vers une matrice
matrice* Creer_matrice(int n, int m)
{
    matrice *p = new matrice; //pointeur qui seras de type matrice

    (*p).nbl = n + 2;
    (*p).nbc = m + 2;
    (*p).T = Creer_tab2D(n, m);

    initZero_mat(*p);
    
    return(p);
}




/* GROUPE MATRICES DU JEUX */

//  Initialise la matrice mines 
matrice* Int_jeu(int n, int m, int difficult)
{
    matrice* T = Creer_matrice(n, m);

    /* initialize random seed: */
    srand (time(NULL));
    for (int i = 1; i < (*T).nbl - 1; i++)
    {
        for (int j = 1; j < (*T).nbc - 1; j++)
        {
            if ((rand() % 10 + 1) > difficult)
            {
                (*T).T[i][j] = 2;
            }
        }
    }
    return T;
}

//  utile pour l'initialisation
int calc_mines_autour(int i, int j, matrice* all_matrice[])
{
    int calc = 0;
    for (int n = i - 1; n <= i + 1; n++)
    {
        for (int m = j - 1; m <= j + 1; m++)
        {
            if (((n != i)||(m != j))&&((*all_matrice[0]).T[n][m] == 2))
            {
                calc = calc + 1;
            }
        }
    }
    return calc;
}

//  Initialise la matrice mines_autour
matrice* mines_autour(int n, int m, matrice* all_matrice[])
{
    matrice* M = Creer_matrice(n, m);

    for (int i = 1; i < (*all_matrice[0]).nbl - 1; i++)
    {
        for (int j = 1; j < (*all_matrice[0]).nbc - 1; j++)
        {
            (*M).T[i][j] = calc_mines_autour(i, j, all_matrice);
        }
    }
    
    return M;
}

//  Initialise la matrice mines_degat 
matrice* degats_mines(int n, int m, matrice* all_matrice[])
{
    matrice* T = Creer_matrice(n, m);

    /* initialize random seed: */
    srand (time(NULL));
    for (int i = 1; i < (*T).nbl - 1; i++)
    {
        for (int j = 1; j < (*T).nbc - 1; j++)
        {
            if ((*all_matrice[0]).T[i][j] == 2)
            {
                (*T).T[i][j] = (rand() % 3 + 1);
            }
        }
    }
    return T;
}




/* GROUPE ACTION DU JEUX */

// 1-   Pose un drapeau sur une Matrice action qui sera lu pour etre afficher
void poser_drapeau(int i, int j, matrice* all_matrice[])
{
    if ((*all_matrice[2]).T[i][j] == 0)
    {
        (*all_matrice[3]).T[i][j] = 1;
    }
}

// 2-   Enleve un drapeau sur une Matrice action qui sera lu pour etre afficher 
void enlever_drapeau(int i, int j, matrice* all_matrice[])
{
    if ((*all_matrice[3]).T[i][j] == 1)
    {
        (*all_matrice[3]).T[i][j] = 0;
    }
}

//      Pose un drapeau sur une Matrice action qui sera lu pour etre afficher de manière recursive       // Jamais l'utiliser directement
void creuser_cases_autour(int i, int j, matrice* all_matrice[])
{
    if ((i >= 1) && (i < ((*all_matrice[0]).nbl - 1)) && (j >= 1) && (j < ((*all_matrice[0]).nbc - 1)))
    {
        (*all_matrice[2]).T[i][j] = 1; //je creuse

        for (int f = i - 1; f <= i + 1; f++)
        {
            for (int g = j - 1; g <= j + 1; g++) //pour chaque cases autour de i et j
            {
                if (((*all_matrice[1]).T[f][g] == 0)&&((*all_matrice[2]).T[f][g] == 0))
                {
                    creuser_cases_autour(f, g, all_matrice);
                }
            }
        }

        for (int f = i - 1; f <= i + 1; f++)
        {
            for (int g = j - 1; g <= j + 1; g++) //pour chaque cases autour de i et j
            {
                (*all_matrice[2]).T[f][g] = 1; //je creuse
            }
        }
    }
}

// 3-   Creuse une sur une Matrice action qui sera lu pour etre afficher
bool creuser(int i, int j, matrice* all_matrice[])
{
    if (((*all_matrice[3]).T[i][j] == 0)&&((*all_matrice[5]).T[i][j] == 0)&&((*all_matrice[2]).T[i][j] == 0))
    {
        if ((*all_matrice[0]).T[i][j] == 2) //si il y a une mine
        {
            (*all_matrice[2]).T[i][j] = 3;
            return false;
        }
        else if ((*all_matrice[1]).T[i][j] == 0) //si il y a pas de mine autour de la case
        {
            creuser_cases_autour(i, j, all_matrice); //jutilise une fonction recursive
            //
            return true;
        }
        else //si il ny a pas de mine
        {
            (*all_matrice[2]).T[i][j] = 1;
            return true;
        }
    }
    return true;
}

// 4-   Utiliser un joker pour afficher sans degat potentiel
void poser_joker(int i, int j, matrice* all_matrice[])
{
    (*all_matrice[5]).T[i][j] = 1; 

    if ((*all_matrice[0]).T[i][j] != 2)//sauf si c'est une mine
    {
        (*all_matrice[2]).T[i][j] = 1; //consider comme creuser
    }

    // 0 1
    // 1 1
}




/* GROUPE INITIALISATION */

//   Initialise les Matrices de jeux et Initialise les matrices d'action
void init_all_matrice(Pre_Set &p, matrice* all_matrice[])
{
    //Matrice jeux
    all_matrice[0]  = Int_jeu(p.n, p.m, p.d); //mines
    all_matrice[1]  = mines_autour(p.n, p.m, all_matrice); //mines_a
    all_matrice[4]  = degats_mines(p.n, p.m, all_matrice); //degat_mines
    //Matrice action
    all_matrice[2]  = Creer_matrice(p.n, p.m); //creuser
    all_matrice[3]  = Creer_matrice(p.n, p.m); //drapeau
    all_matrice[5]  = Creer_matrice(p.n, p.m); //joker
}

//   Initialise les valeurs utiles du jeu
void init_preset(Pre_Set &p)
{
    p.compteur_cases = p.n * p.m;
    p.score = 0;

    if (p.d <= 5)
    {
        p.init_life = 6;
    }
    else if ((p.d > 5)&&(p.d < 9))
    {
        p.init_life = 9;
    }
    else if (p.d == 9)
    {
        p.init_life = 12;
    }
    p.life = p.init_life;
}

//   Configure le jeux
void game_setting(Pre_Set &p)
{
    cout<<"******************\n";
    cout<<"//   DEMINEUR   //\n";
    cout<<"******************\n";
    cout<<"\nNombre de Ligne    : ";
    cin>>p.n;
    cout<<"\nNombre de Collonne : ";
    cin>>p.m;
    cout<<"\nDifficulter (1 hard, 9 easy) :";
    cin>>p.d;
}

//   Init parametre du jeux //Lance l'initialisation des Matrices //Lance la configuration du jeux
void Init_game(Pre_Set &p, matrice* all_matrice[])
{
    cout<<system("cls")<<endl;

    game_setting(p);
    init_preset(p);
    init_all_matrice(p, all_matrice);

    p.game = true;
}




/* GROUPE BOUCLE IS GAGNER ? */

//  Compte le nombre de cases creuse     
void Compteur_creuser(Pre_Set &p, matrice* all_matrice[])
{
    p.compteur_creuser = 0;
    for (int i = 1; i < (*all_matrice[2]).nbl - 1; i++)
    {
        for (int j = 1; j < (*all_matrice[2]).nbc - 1; j++)
        {
            if ((*all_matrice[2]).T[i][j] == 1)
            {
                p.compteur_creuser = p.compteur_creuser + 1;
            }
        }
    }
}

//  Compte le nombre de Joker utiliser     
void Compteur_joker(Pre_Set &p, matrice* all_matrice[])
{
    p.compteur_joker = 0;
    for (int i = 1; i < (*all_matrice[5]).nbl - 1; i++)
    {
        for (int j = 1; j < (*all_matrice[5]).nbc - 1; j++)
        {
            if ((*all_matrice[5]).T[i][j] == 1)
            {
                p.compteur_joker = p.compteur_joker + 1;
            }
        }
    }
}

//  Compte le nombre de cases miner      
void Compteur_mines(Pre_Set &p, matrice* all_matrice[])
{
    p.compteur_mines = 0;
    for (int i = 1; i < (*all_matrice[0]).nbl - 1; i++)
    {
        for (int j = 1; j < (*all_matrice[0]).nbc - 1; j++)
        {
            if ((*all_matrice[0]).T[i][j] == 2)
            {
                p.compteur_mines = p.compteur_mines + 1;
            }
        }
    }
}

//  En fonction des compteurs verifie si on a gagner ou non
bool is_win(Pre_Set &p, matrice* all_matrice[])
{
    int tamp;
    Compteur_creuser(p, all_matrice);
    Compteur_mines(p, all_matrice);
    
    //gagne
    if ((p.compteur_cases - (p.compteur_creuser + p.compteur_mines)) == 0)
    {
        return true;
    }
    //pas gagner
    else
    {
        return false;
    }
}




/* NON TRIER */

//
void affiche_score(Pre_Set &p)
{
    p.score = (p.compteur_cases * p.compteur_mines * p.compteur_creuser);
    if ((p.init_life - p.life) >= 0)
    {
        p.score = p.score/(p.init_life + 1 - p.life);
    }
    else if ((p.init_life - p.life) < 0)
    {
        p.score = p.score/p.init_life;
    }

    if (p.compteur_joker != 0)
    {
        p.score/(p.compteur_joker + 1);
    }
    
    cout<<"\nScore :    "<<p.score<<endl;
}

//  return true si encore en vie
bool prendre_degats(int i, int j, Pre_Set &p, matrice* all_matrice[])
{
    //je prend des degats
    p.life = p.life - (*all_matrice[4]).T[i][j];

    //si j'ai plus de vie
    if (p.life <= 0)
    {
        return false; //on dit que j'ai plus de vie
    }
    else
    {
        return true; //on dit que j'ai encore de la vie
    }
}

//  Affichage du menu   //return le choix
int choix_action(Pre_Set &p)
{
    int choix;

    cout<<"\n******************\n";
    cout<<"//    CHOIX     //\n";
    cout<<"******************\n";

    cout<<"\nVie  :  ";
    cout<<p.life<<endl;

    cout<<"\n1 Creuser:               ";
    cout<<"2 Poser Drapeau  :       ";
    cout<<"3 Enlever Drapeau:       ";
    cout<<"4 Utiliser un joker:     ";
    cout<<"5 Recommencer:            \n";
    
    cin>>choix;

    return choix;
}

//  return des coordonnés   //utile pour demander au joueur quel case il choisi
void return_cord(int &i, int &j)
{
    cout<<"\nCoordonnees x, y :\n";
    cin>>i;
    cin>>j;
}




/* GROUPE AFFICHAGE */
matrice* init_matrice_affichage(matrice* all_matrice[])
{
    int nbl = (*all_matrice[0]).nbl;
    int nbc = (*all_matrice[0]).nbc;

    matrice* G = Creer_matrice(nbl - 2, nbc - 2);

    //on donne les valeurs a la matrice d'affichage en fonction des autres matrices
    for (int i = 1; i < nbl - 1; i++)
    {
        for (int j = 1; j < nbc - 1; j++)
        {
            if (((*all_matrice[2]).T[i][j] == 0) && ((*all_matrice[3]).T[i][j] == 0) && ((*all_matrice[5]).T[i][j] == 0))
            {
                (*G).T[i][j] = 500;
            }
            else if (((*all_matrice[2]).T[i][j] == 0) && ((*all_matrice[3]).T[i][j] == 1))
            {
                (*G).T[i][j] = 501;
            }
            else if (((*all_matrice[2]).T[i][j] == 0) || ((*all_matrice[2]).T[i][j] == 1) && ((*all_matrice[5]).T[i][j] == 1))
            {
                (*G).T[i][j] = 502;
            }
            else if (((*all_matrice[2]).T[i][j] == 1) && ((*all_matrice[0]).T[i][j] == 0) && ((*all_matrice[1]).T[i][j] == 0) && ((*all_matrice[5]).T[i][j] == 0))
            {
                (*G).T[i][j] = 503;
            }
            else if (((*all_matrice[2]).T[i][j] == 1) && ((*all_matrice[0]).T[i][j] == 0) && ((*all_matrice[1]).T[i][j] != 0) && ((*all_matrice[5]).T[i][j] == 0))
            {
                (*G).T[i][j] = 504;
            }
            else if ((*all_matrice[2]).T[i][j] == 3)
            {
                (*G).T[i][j] = 505;
            }

        }
    }

    return G;
}

void affichage(matrice* all_matrice[])
{
    matrice* G = init_matrice_affichage(all_matrice);

    //en fonction des valeurs de la matrice d'affichage on renvois des affichages
    for (int i = 0; i < (*all_matrice[0]).nbl - 1; i++)
    {
        for (int j = 0; j < (*all_matrice[0]).nbc - 1; j++)
        {
            //initialisation de numérotation des lignes et des collonnes
            (*G).T[i][0] = i;
            (*G).T[0][j] = j; 

            /* Ici on affiche en alignent corectement les lignes et les colonnes*/
            if ((i < 10)&&(j == 0))
            {
                cout<<" ";
            }
            else if ((j > 10)&&(i != 0))
            {
                cout<<" ";
            }
            else
            {
                cout<<"";
            }

            if ((*G).T[i][j] == 500)
            {
                cout<<"*";
                cout<<" ";
            }
            else if ((*G).T[i][j] == 501)
            {
                cout<<"D";
                cout<<" ";
            }
            else if ((*G).T[i][j] == 502) //joker
            {
                if ((*all_matrice[0]).T[i][j] == 2)
                {
                    cout<<"B";
                    cout<<" ";
                }
                else if (((*all_matrice[1]).T[i][j] == 0)&&((*all_matrice[0]).T[i][j] != 2))
                {
                    cout<<" ";
                    cout<<" ";
                }
                else if (((*all_matrice[1]).T[i][j] != 0)&&((*all_matrice[0]).T[i][j] != 2))
                {
                    cout<<(*all_matrice[1]).T[i][j];
                    cout<<" ";
                }
            }
            else if ((*G).T[i][j] == 503)
            {
                cout<<" ";
                cout<<" ";
            }
            else if ((*G).T[i][j] == 504)
            {
                cout<<(*all_matrice[1]).T[i][j];
                cout<<" ";
            }
            else if ((*G).T[i][j] == 505)
            {
                cout<<"B";
                cout<<" ";
            }
            else
            {
                cout<<(*G).T[i][j];
                cout<<" ";
            }
        }
        cout<<"\n";
    } 
}

// Permet au joueur de decouvrir comment etait le plateau apres avoir perdu ou gagner
void affichage_all(matrice* all_matrice[])
{
    bool afficher = false;
    int nbl = (*all_matrice[0]).nbl;
    int nbc = (*all_matrice[0]).nbc;
    matrice* G = Creer_matrice(nbl - 2, nbc - 2);

    cout<<"\nMontrer la soluce (1 - oui ; 0 - non) :";
    cin>>afficher;

    if (afficher == true)
    {
        //on donne les valeurs a la matrice d'affichage en fonction des autres matrices
        for (int i = 1; i < nbl - 1; i++)
        {
            for (int j = 1; j < nbc - 1; j++)
            {
                if ((*all_matrice[0]).T[i][j] == 2) //miner
                {
                    (*G).T[i][j] = 122;
                }
                else if ((*all_matrice[0]).T[i][j] != 2) //pas miner
                {
                    (*G).T[i][j] = 100;
                }
            }
        }

        //en fonction des valeurs de la matrice d'affichage on renvois des affichages
        for (int i = 0; i < (*all_matrice[0]).nbl - 1; i++)
        {
            for (int j = 0; j < (*all_matrice[0]).nbc - 1; j++)
            {
                //initialisation de numérotation des lignes et des collonnes
                (*G).T[i][0] = i;
                (*G).T[0][j] = j; 

                /* Ici on affiche en alignent corectement les lignes et les colonnes*/
                if ((i < 10)&&(j == 0))
                {
                    cout<<" ";
                }
                else if ((j > 10)&&(i != 0))
                {
                    cout<<" ";
                }
                else
                {
                    cout<<"";
                }

                if ((*G).T[i][j] == 122)
                {
                    cout<<"B";
                    cout<<" ";
                }
                else if ((*G).T[i][j] == 100)
                {
                    cout<<" ";
                    cout<<" ";
                }
                else
                {
                    cout<<(*G).T[i][j];
                    cout<<" ";
                }
            }
            cout<<"\n";
        }    
    }
}




/* GROUPE BOUCLE GAME */

// Permet au joueur de choisir ce qu'il veut faire
void game_action(Pre_Set &p, matrice* all_matrice[])
{
    int choix;
    int i, j;

    choix = choix_action(p);

    if (choix == 1)
    {
        return_cord(i, j);
        
        //si je tombe sur une mine
        if (creuser(i, j, all_matrice) == false)
        {
            
            p.game = prendre_degats(i, j, p, all_matrice); //on dit que j'ai perdu
        }
        cout<<"\n\n\n";
    }
    else if (choix == 2)
    {
        return_cord(i, j);

        poser_drapeau(i, j, all_matrice);
        cout<<"\n\n\n";
    }
    else if (choix == 3)
    {
        return_cord(i, j);

        enlever_drapeau(i, j, all_matrice);
        cout<<"\n\n\n";
    }
    else if (choix == 4)
    {
        Compteur_joker(p, all_matrice);
        if (p.compteur_joker <= 3)
        {
            return_cord(i, j);
            poser_joker(i, j, all_matrice);
        }
        cout<<"\n\n\n";
    }
    else if (choix == 5)
    {
        p.game = false;
        cout<<"\n\n\n";
    }
}

// Propose de rejouer si appeler
void replay(Pre_Set &p, matrice* all_matrice[])
{
    cout<<"Veut tu rejouer ? (1 - oui ; 0 - non) : ";
    cin>>p.game;

    if (p.game == true)
    {
        Init_game(p, all_matrice); //pour rejouer il suffit de reinitialiser le jeux
    }
}

// 1- Lance l'affichage 2- Lance le game_action 3- Verifie si Win or Lose 4- Relance le jeux avec une nouvelle Initialisation   ou pas
void boucle_game(Pre_Set &p, matrice* all_matrice[])
{
    while (p.game == true)
    {
        /* Action du jeux  */

        affichage(all_matrice);         //affiche le tableaux
        game_action(p, all_matrice);    //propose au joueur un choix d'action


        /* GAGNER ? PERDU ? */

        //  Si perdu --> on propose d'afficher la soluce --> on propose de rejouer
        if (p.game == false)
        {
            cout<<system("cls")<<endl;

            affichage(all_matrice);
            
            cout<<"\n\n*******PERDU*******\n\n";

            affiche_score(p);
            affichage_all(all_matrice);
            replay(p, all_matrice);     
        }
        
        //  Si pas encore gagner --> on continue la boucle
        if ((p.game == true)&&(is_win(p, all_matrice) == false))
        {
            cout<<system("cls")<<endl;
        }
        //  Si gagner --> on propose de rejouer
        else if ((p.game == true)&&(is_win(p, all_matrice) == true))
        {
            cout<<system("cls")<<endl;

            affichage(all_matrice);

            cout<<"\n\n*****GAGNER*******\n\n";

            affiche_score(p);
            affichage_all(all_matrice);
            replay(p, all_matrice);    
        }
    }
}
