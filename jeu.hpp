#ifndef JEU_H
#define JEU_H

#include "moteur.hpp"
#include "niveau.hpp"


class Jeu
{
private:
	Moteur mMoteurPrincipal;
	Niveau mNiveau;
public:
	Jeu() = default;
	bool chargerNiveau( unsigned int numNiv );
	void lancerJeu();
};

#endif // JEU_H
