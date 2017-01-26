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
	void earnInput();
	GestionnaireECS &getGestionnaireECS();
	void loadTileMap( const std::string & pathTile );
	bool loadPlayersAndBot( unsigned int uiNumPlayer, unsigned int uiNumBot );
};

#endif // MOTEUR_H
