#ifndef MOTEURGRAPHIQUE_H
#define MOTEURGRAPHIQUE_H

class Moteur;

#include <SFML/Graphics.hpp>
#include "tilemap.hpp"

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
public:
	MoteurGraphique() = default;
	void initialiserFenetre();
	void linkMainEngine( Moteur* ptrMoteur );
	bool loadTileMap( const std::string &strPathConfFile );
	void raffraichirEcran();
};

#endif // MOTEURGRAPHIQUE_H
