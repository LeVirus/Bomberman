#include "jeu.hpp"

bool Jeu::chargerNiveau( unsigned int numNiv )
{
	return mMoteurPrincipal.loadTileMap( mNiveau.getConfLevelTileMap( numNiv ) );
}

void Jeu::lancerJeu()
{
	mMoteurPrincipal.lancerBoucle();
}
