#include <iostream>
#include "jeu.hpp"
#include "basesocket.hpp"

GameMode setConfig();

int main()
{
    GameMode input = setConfig();
    if(input == GameMode::SERVER)//TEST
    {
        std::cout << "SERVER\n";
        BaseSocket base;
        base.setListener();
        std::cout << "listen on port 54000\n";
        base.checkReceiveData();
    }
    else if(input == GameMode::CLIENT)//TEST
    {
        std::cout << "CLIENT\n";
        BaseSocket base;
        base.sendData("127.0.0.1", 54000);
    }
//    else {
        Jeu jeu;
        jeu.initECS(input);
        do
        {
            jeu.chargerNiveau(0, input);
            jeu.lancerJeu();
        }while(! jeu.getMainEngine().stopGame());
//    }
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
    if(input == 0)
    {
        ret = GameMode::CLIENT;
    }
    else
    {
        ret = GameMode::SOLO;
    }
    return ret;
}
