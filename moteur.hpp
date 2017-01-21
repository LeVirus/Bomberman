#ifndef MOTEUR_H
#define MOTEUR_H

#include "gestionnaireecs.hpp"
#include "moteurgraphique.hpp"
#include <string>

class Moteur
{
private:
	GestionnaireECS mGestECS;
	MoteurGraphique mMoteurGraphique;
public:
	Moteur();
	void lancerBoucle();
	GestionnaireECS &getGestionnaireECS();
	bool loadTileMap( const std::string & pathTile );
};

#endif // MOTEUR_H
