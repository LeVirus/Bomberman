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
	void positionnerComponent(ecs::PositionComponent &posComp, unsigned int posX, unsigned int posY,
							  unsigned int taileTileX, unsigned int taileTileY);
	const Jeu &mPtrJeu;
public:
	Moteur(const Jeu &jeu);
	void linkJeu(Jeu &jeu);
	const Jeu &getJeu()const;
	void lancerBoucle();
	void earnInput();
	GestionnaireECS &getGestionnaireECS();
	void loadTileMap(const Niveau &niv);
	bool loadPlayersAndBot( unsigned int uiNumPlayer, unsigned int uiNumBot );
	void loadLevelWall(const Niveau &niv);
};

#endif // MOTEUR_H
