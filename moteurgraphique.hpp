#ifndef MOTEURGRAPHIQUE_H
#define MOTEURGRAPHIQUE_H

class Moteur;

#include "tilemap.hpp"
#include "niveau.hpp"

namespace ecs
{
    struct PositionComponent;
    struct DisplayComponent;
}

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
    static float mCaseLenght, mCaseHeight;
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
    static void static_positionComponentCenterCurrentCase(ecs::PositionComponent &positionComp);
};

#endif // MOTEURGRAPHIQUE_H
