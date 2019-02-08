#ifndef MOTEUR_H
#define MOTEUR_H

#include "gestionnaireecs.hpp"
#include "moteurgraphique.hpp"
#include "niveau.hpp"
#include <string>

class Jeu;
class SocketSystem;
class Tableau2D;
struct NetworkLevelData;

class Moteur
{
private:
	GestionnaireECS mGestECS;
	MoteurGraphique mMoteurGraphique;
    const Jeu &mPtrJeu;
    bool mLetMeOut = false;

    void positionnerComponent(ecs::PositionComponent &posComp, uint32_t posX, uint32_t posY);
    uint32_t createBomberman(uint32_t numPlayer);
    void fillBombermanEntityBitset(std::vector<bool> &bombermanBitset,
                                   uint32_t uiNumPlayer);
    uint32_t instanciateBombermanComponents(std::vector<bool> &bombermanBitset);
    void configBombermanComponents(uint32_t numEntity, uint32_t numPlayer,
                                   const std::vector<bool> &bombermanBitset);
    void synchLevelFromServer(SocketSystem &socketSystem, Niveau &niv);
    void synchPlayersFromServer(SocketSystem &socketSystem);
public:
	Moteur(const Jeu &jeu);
    uint32_t initLevel();
	const Jeu &getJeu()const;
    SocketSystem *getSocketSystem();
	void lancerBoucle();
	void getInput();
    void loadLevelTileMap(Niveau &niv, uint32_t numNiv);
	GestionnaireECS &getGestionnaireECS();
    bool loadPlayersAndBot(uint32_t uiNumPlayer, uint32_t uiNumBot);
    void loadLevelWall(const Niveau &niv);
    void waitForClientsConnect();
    void waitServerSync(Niveau &niv);
    void synchronizeLevelToClients(const Niveau &level);

    /**
     * @brief synchronizeEntitiesNetworkId
     * Send data from server to client. This for synch
     * entities network id to client and server.
     */
    void synchronizeEntitiesNetworkIdToClients();
    bool stopGame()const;
};

#endif // MOTEUR_H
