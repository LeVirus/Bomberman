#ifndef MOTEUR_H
#define MOTEUR_H

#include "gestionnaireecs.hpp"
#include "moteurgraphique.hpp"
#include "niveau.hpp"
#include <string>

class Jeu;

class Moteur
{
private:
	GestionnaireECS mGestECS;
	MoteurGraphique mMoteurGraphique;
	void positionnerComponent(ecs::PositionComponent &posComp, unsigned int posX, unsigned int posY);
    void loadBombermanSprite();
	const Jeu &mPtrJeu;
    bool mLetMeOut = false;
public:
	Moteur(const Jeu &jeu);
	const Jeu &getJeu()const;
	void lancerBoucle();
	void getInput();
	GestionnaireECS &getGestionnaireECS();
    void loadLevelTileMap(Niveau &niv, unsigned int numNiv);
	bool loadPlayersAndBot( unsigned int uiNumPlayer, unsigned int uiNumBot );
    void loadLevelWall(const Niveau &niv);
    bool stopGame()const;
};

#endif // MOTEUR_H
