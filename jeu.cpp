#include "jeu.hpp"
#include <cassert>

bool Jeu::chargerNiveau( unsigned int numNiv )
{
	mMoteurPrincipal.getGestionnaireECS().initECS();
	mMoteurPrincipal.loadTileMap( mNiveau.getConfLevelTileMap( numNiv ) );
	mMoteurPrincipal.loadPlayersAndBot( 1,0 );
	mMoteurPrincipal.loadLevelWall();
	return true;
}

void Jeu::lancerJeu()
{
	mMoteurPrincipal.lancerBoucle();
}
