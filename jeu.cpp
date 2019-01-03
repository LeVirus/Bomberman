#include "jeu.hpp"
#include <cassert>

Jeu::Jeu():mMoteurPrincipal(*this)
{

}

bool Jeu::chargerNiveau(unsigned int numNiv, GameMode gm)
{
    mMoteurPrincipal.loadLevelTileMap(mNiveau, numNiv);
    mMoteurPrincipal.loadPlayersAndBot(2, 0);
	mMoteurPrincipal.loadLevelWall(mNiveau);
    if(gm == GameMode::SERVER)
    {
        mMoteurPrincipal.synchronizeEntitiesNetworkId();
    }
    return true;
}

void Jeu::initECS(GameMode mod)
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
