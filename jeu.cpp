#include "jeu.hpp"
#include "socketsystem.hpp"

GameMode Jeu::mGameMode;

Jeu::Jeu(GameMode gm) : mMoteurPrincipal(*this)
{
    mGameMode = gm;
}

bool Jeu::chargerNiveau(unsigned int numNiv)
{
    if(mGameMode != GameMode::CLIENT)
    {
        mMoteurPrincipal.loadLevelTileMap(mNiveau, numNiv);
        mMoteurPrincipal.loadPlayersAndBot(2, 0);
        mMoteurPrincipal.loadLevelWall(mNiveau);
    }
    if(mGameMode == GameMode::SERVER)
    {
        waitForClientsConnect();
        mMoteurPrincipal.synchronizeLevelToClients(mNiveau);
        mMoteurPrincipal.synchronizeEntitiesNetworkIdToClients();
        mMoteurPrincipal.getSocketSystem()->launchReceptThread();
    }
    else if(mGameMode == GameMode::CLIENT)
    {
        mMoteurPrincipal.waitServerSync(mNiveau);
        mMoteurPrincipal.loadLevelWall(mNiveau);
    }
    return true;
}

void Jeu::waitForClientsConnect()
{
    bool launch;
    std::cout << "Type anything to launch game." << std::endl;
    std::cin >> launch;
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
