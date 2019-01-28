#include <iostream>
#include "jeu.hpp"
#include "basesocket.hpp"

GameMode setConfig();

int main()
{
    GameMode input = setConfig();

    Jeu jeu(input);

    jeu.initECS();

    do
    {
        jeu.chargerNiveau(0);
        jeu.lancerJeu();
    }while(! jeu.getMainEngine().stopGame());
	return 0;
}

GameMode setConfig()
{
    unsigned int input;
    GameMode ret;
    do
    {
        std::cout << "(0) SERVER\n(1) CLIENT\n(2) SOLO\n";
        std::cin >> input;
    }while (input > 2);
    if(input == 0)
    {
        ret = GameMode::SERVER;
    }
    else if(input == 1)
    {
        ret = GameMode::CLIENT;
    }
    else
    {
        ret = GameMode::SOLO;
    }
    return ret;
}
