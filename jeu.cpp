#include "jeu.hpp"
#include <cassert>

Jeu::Jeu():mMoteurPrincipal(*this)
{

}

bool Jeu::chargerNiveau(unsigned int numNiv)
{
	mMoteurPrincipal.getGestionnaireECS().initECS();
    mMoteurPrincipal.loadLevelTileMap(mNiveau, numNiv);
    mNiveau.adaptToScale(SIZE_SCALE, SIZE_SCALE);
    mMoteurPrincipal.loadPlayersAndBot(1, 0);
	mMoteurPrincipal.loadLevelWall(mNiveau);
	return true;
}

const Niveau &Jeu::getNiveau() const
{
	return mNiveau;
}

void Jeu::lancerJeu()
{
	mMoteurPrincipal.lancerBoucle();
}
