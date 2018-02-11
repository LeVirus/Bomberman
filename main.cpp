#include <iostream>
#include "jeu.hpp"


int main()
{
	Jeu jeu;
    jeu.initECS();
    do
    {
        jeu.chargerNiveau(0);
        jeu.lancerJeu();
    }while(! jeu.getMainEngine().stopGame());

	return 0;
}
