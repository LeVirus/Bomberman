#ifndef JEU_H
#define JEU_H

#include "moteur.hpp"


class Jeu
{
private:
	Moteur mMoteurPrincipal;
public:
	Jeu() = default;
	void chargerNiveau( unsigned int numNiv );
	void lancerJeu();
};

#endif // JEU_H
