#ifndef MOTEURGRAPHIQUE_H
#define MOTEURGRAPHIQUE_H

class Moteur;

#include <SFML/Graphics.hpp>
#include "tilemap.hpp"
#include "displaycomponent.hpp"
#include "niveau.hpp"
#include "positioncomponent.hpp"

class MoteurGraphique
{
private:
	std::vector< sf::Sprite > mVectSprite;
	sf::RenderWindow mFenetre;
	std::vector< sf::Texture > mVectTexture;
	sf::View mCamera;
	sf::VertexArray mVertArrayTileMap;
	TileMap mTileMap;
	Moteur* mPtrMoteurPrincipal = nullptr;
	const std::vector< std::pair< ecs::DisplayComponent *, ecs::PositionComponent * > > *
			mVectComponentDisplaySystem;
public:
	MoteurGraphique();
    void getEventFromWindows(sf::Event &event);
	const TileMap &getTileMap()const;
	void initialiserFenetre();
	void linkMainEngine( Moteur* ptrMoteur );
	void loadTileMap(const Niveau &level, unsigned int uiNumEntity );
	unsigned int loadSprite(unsigned int uiNumTexture, const sf::IntRect &positionSprite );
	void raffraichirEcran();
	void displayECSSprite();
	void positionnerCaseTileMap(unsigned int uiNumEntity, unsigned int uiPositionX, unsigned int uiPositionY );
};

#endif // MOTEURGRAPHIQUE_H
