#include <iostream>
#include "jeu.hpp"

unsigned int setConfig();

int main()
{
    unsigned int input = setConfig();
    if(input == SERVER)
    {

    }
    else if(input == CLIENT)
    {

    }
    else {
        Jeu jeu;
        jeu.initECS();
        do
        {
            jeu.chargerNiveau(0);
            jeu.lancerJeu();
        }while(! jeu.getMainEngine().stopGame());
    }
	return 0;
}

unsigned int setConfig()
{
    unsigned int input;
    do
    {
        std::cout << "(0) SERVER\n(1) CLIENT\n(2) SOLO\n";
        std::cin >> input;
    }while (input > 2);
    return input;
}
