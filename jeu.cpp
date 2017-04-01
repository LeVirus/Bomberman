#include "jeu.hpp"
#include <cassert>

bool Jeu::chargerNiveau( unsigned int numNiv )
{
	mNiveau.loadLevel(numNiv);
	mNiveau.displayLevel();
	mMoteurPrincipal.getGestionnaireECS().initECS();
	mMoteurPrincipal.loadTileMap( );
	mMoteurPrincipal.loadPlayersAndBot( 1, 0 );
	mMoteurPrincipal.loadLevelWall();
	return true;
}

void Jeu::lancerJeu()
{
	mMoteurPrincipal.lancerBoucle();
}
