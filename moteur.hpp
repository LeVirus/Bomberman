#ifndef MOTEUR_H
#define MOTEUR_H

#include "gestionnaireecs.hpp"
#include "moteurgraphique.hpp"
#include "niveau.hpp"
#include <string>

class Moteur
{
private:
	GestionnaireECS mGestECS;
	MoteurGraphique mMoteurGraphique;
	Niveau mNiveau;

	void positionnerComponent(ecs::PositionComponent &posComp, unsigned int posX, unsigned int posY,
							  unsigned int taileTileX, unsigned int taileTileY);
public:
	Moteur();
	void lancerBoucle();
	void earnInput();
	GestionnaireECS &getGestionnaireECS();
	void loadTileMap( const std::string & pathTile );
	bool loadPlayersAndBot( unsigned int uiNumPlayer, unsigned int uiNumBot );
	void loadLevelWall();
	const Niveau &getNiveau()const;
};

#endif // MOTEUR_H
