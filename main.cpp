#include <iostream>
#include "jeu.hpp"
#include "basesocket.hpp"

unsigned int setConfig();

int main()
{
    unsigned int input = setConfig();
    if(input == SERVER)//TEST
    {
        std::cout << "SERVER\n";
        BaseSocket base;
        base.setListener();
        std::cout << "listen on port 54000\n";
//        base.checkReceiveData();
    }
    else if(input == CLIENT)//TEST
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
            jeu.chargerNiveau(0);
            jeu.lancerJeu();
        }while(! jeu.getMainEngine().stopGame());
//    }
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
