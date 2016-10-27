#ifndef MOTEUR_H
#define MOTEUR_H

#include "gestionnaireecs.hpp"
#include "moteurgraphique.hpp"


class Moteur
{
private:
	GestionnaireECS mGestECS;
	MoteurGraphique mMoteurGraphique;
public:
	Moteur();
	void lancerBoucle();
};

#endif // MOTEUR_H
