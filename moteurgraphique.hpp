#ifndef MOTEURGRAPHIQUE_H
#define MOTEURGRAPHIQUE_H

class Moteur;

#include "tilemap.hpp"
#include <vector>

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
    std::vector<TileMap> mVectTileMap;
	Moteur* mPtrMoteurPrincipal = nullptr;
	const std::vector< std::pair< ecs::DisplayComponent *, ecs::PositionComponent * > > *
			mVectComponentDisplaySystem;
    static float mCaseLenght, mCaseHeight;
public:
	MoteurGraphique();
    void getEventFromWindows(sf::Event &event);
    const std::vector<TileMap> &getTileMap()const;
	void initialiserFenetre();
	void linkMainEngine( Moteur* ptrMoteur );
    //void loadLevelTileMap(const Niveau &level, unsigned int uiNumEntity );
	unsigned int loadSprite(unsigned int uiNumTexture, const sf::IntRect &positionSprite );
	void raffraichirEcran();
    void displayECSTilemap();
	void displayECSSprite();
    static void static_positionnerCaseTileMap(ecs::PositionComponent &posComp, unsigned int uiPositionX, unsigned int uiPositionY );
    static void static_positionComponentCenterCurrentCase(ecs::PositionComponent &positionComp);
    static void static_getPositionsCurrentCase(const ecs::PositionComponent &positionComp,
                                               unsigned int &caseX, unsigned int &caseY, bool launchBomb = true);
    void memorizeSizeTile(unsigned int lenghtTile, unsigned int heightTile);
};

#endif // MOTEURGRAPHIQUE_H
