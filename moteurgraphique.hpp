#ifndef MOTEURGRAPHIQUE_H
#define MOTEURGRAPHIQUE_H

class Moteur;

#include <SFML/Graphics.hpp>
#include "tilemap.hpp"
#include "displaycomponent.hpp"
#include "positioncomponent.hpp"

class MoteurGraphique
{
private:
	std::vector< sf::Sprite > mVectSprite;
	sf::RenderWindow mFenetre;
	std::vector< sf::Texture > mVectTexture;
	sf::View mCamera;
	sf::VertexArray mVertArrayTileMap;
	std::pair< unsigned int, unsigned int> mPairPositionLevel;
	TileMap mTileMap;
	Moteur* mPtrMoteurPrincipal = nullptr;
	const std::vector< std::pair< ecs::DisplayComponent *, ecs::PositionComponent * > > *
			mVectComponentDisplaySystem;
public:
	MoteurGraphique();
	const std::pair< unsigned int, unsigned int> &getPositionLevel()const;
	void initialiserFenetre();
	void linkMainEngine( Moteur* ptrMoteur );
	bool loadTileMap( const std::string &strPathConfFile, unsigned int uiNumEntity );
	unsigned int loadSprite(unsigned int uiNumTexture, const sf::IntRect &positionSprite );
	void raffraichirEcran();
	void displayECSSprite();
	void positionnerTileMap( unsigned int uiPosition );
};

#endif // MOTEURGRAPHIQUE_H
