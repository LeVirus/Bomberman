#include <iostream>
#include "jeu.hpp"


int main(/*int argc, char *argv[]*/)
{
	Jeu jeu;
	jeu.chargerNiveau( 0 );
	jeu.lancerJeu();
	return 0;
}
