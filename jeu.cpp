#include "jeu.hpp"
#include <cassert>

bool Jeu::chargerNiveau( unsigned int numNiv )
{
	mMoteurPrincipal.getGestionnaireECS().initECS();
	assert(mMoteurPrincipal.loadTileMap( mNiveau.getConfLevelTileMap( numNiv ) )
		   && "error load tilemap\n");
	mMoteurPrincipal.loadPlayersAndBot( 1,0 );
	return true;
}

void Jeu::lancerJeu()
{
	mMoteurPrincipal.lancerBoucle();
}
