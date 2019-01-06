#ifndef MOTEUR_H
#define MOTEUR_H

#include "gestionnaireecs.hpp"
#include "moteurgraphique.hpp"
#include "niveau.hpp"
#include <string>

class Jeu;
class SocketSystem;
class Moteur
{
private:
	GestionnaireECS mGestECS;
	MoteurGraphique mMoteurGraphique;
    const Jeu &mPtrJeu;
    bool mLetMeOut = false;

	void positionnerComponent(ecs::PositionComponent &posComp, unsigned int posX, unsigned int posY);
    void loadBombermanSprite();
    void LoadBomBSprite();
    SocketSystem *getSocketSystem();
public:
	Moteur(const Jeu &jeu);
	const Jeu &getJeu()const;
	void lancerBoucle();
	void getInput();
	GestionnaireECS &getGestionnaireECS();
    void loadLevelTileMap(Niveau &niv, unsigned int numNiv);
    bool loadPlayersAndBot(unsigned int uiNumPlayer, unsigned int uiNumBot);
    void loadLevelWall(const Niveau &niv);
    void waitServerSync();

    /**
     * @brief synchronizeEntitiesNetworkId
     * Send data from server to client. This for synch
     * entities network id to client and server.
     */
    void synchronizeEntitiesNetworkId();
    bool stopGame()const;
};

#endif // MOTEUR_H
