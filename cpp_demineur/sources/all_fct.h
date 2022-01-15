#ifndef ALL_FCT_H
#define ALL_FCT_H

struct Pre_Set
{
    int n;
    int m;
    int d;
    int compteur_mines;
    int compteur_cases;
    int compteur_creuser;
    int compteur_joker;
    int score;

    int init_life;
    int life;

    bool game;
};


struct matrice
{
    int nbl;
    int nbc;
    int** T;
};

void Init_game(Pre_Set &p, matrice* all_matrice[]);

void boucle_game(Pre_Set &p, matrice* all_matrice[]);

#endif // ALL_FCT_H