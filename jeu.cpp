#include "jeu.hpp"
#include <cassert>

GameMode Jeu::mGameMode;

Jeu::Jeu(GameMode gm) : mMoteurPrincipal(*this)
{
    mGameMode = gm;
    std::cerr << gm << mGameMode << "HAHA\n";
}

bool Jeu::chargerNiveau(unsigned int numNiv)
{
    mMoteurPrincipal.loadLevelTileMap(mNiveau, numNiv);
    mMoteurPrincipal.loadPlayersAndBot(2, 0);
    mMoteurPrincipal.loadLevelWall(mNiveau);
    if(mGameMode == GameMode::SERVER)
    {
        mMoteurPrincipal.synchronizeEntitiesNetworkId();
    }
    return true;
}

void Jeu::initECS()
{
    mMoteurPrincipal.getGestionnaireECS().initECS();
}

const Niveau &Jeu::getNiveau() const
{
	return mNiveau;
}

void Jeu::lancerJeu()
{
	mMoteurPrincipal.lancerBoucle();
}
