#include "jeu.hpp"
#include <cassert>

Jeu::Jeu():mMoteurPrincipal(*this)
{

}

bool Jeu::chargerNiveau(unsigned int numNiv)
{
    mMoteurPrincipal.loadLevelTileMap(mNiveau, numNiv);
    mMoteurPrincipal.loadPlayersAndBot(2, 0);
	mMoteurPrincipal.loadLevelWall(mNiveau);
    return true;
}

void Jeu::initECS(unsigned int mod)
{
    mMoteurPrincipal.getGestionnaireECS().initECS(mod);
}

const Niveau &Jeu::getNiveau() const
{
	return mNiveau;
}

void Jeu::lancerJeu()
{
	mMoteurPrincipal.lancerBoucle();
}
