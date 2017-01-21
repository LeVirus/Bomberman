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
	std::vector< std::unique_ptr< sf::Sprite > > mVectSprite;
	sf::RenderWindow mFenetre;
	sf::Texture textureA, textureNiveau;
	sf::View mCamera;
	sf::VertexArray mVertArrayTileMap;
	TileMap mTileMap;
	Moteur* mPtrMoteurPrincipal = nullptr;
	 const std::vector< std::pair< ecs::DisplayComponent *, ecs::PositionComponent * > > *
			mVectComponentDisplaySystem;
public:
	MoteurGraphique() = default;
	void initialiserFenetre();
	void linkMainEngine( Moteur* ptrMoteur );
	bool loadTileMap( const std::string &strPathConfFile, unsigned int uiNumEntity );
	void raffraichirEcran();
	void displayECSSprite();
	void positionnerTileMap(unsigned int uiPosition);
};

#endif // MOTEURGRAPHIQUE_H
