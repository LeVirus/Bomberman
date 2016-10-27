#ifndef MOTEURGRAPHIQUE_H
#define MOTEURGRAPHIQUE_H

class Moteur;

#include <vector>
#include <SFML/Graphics.hpp>
#include "tilemap.hpp"

class MoteurGraphique
{
private:
	sf::RenderWindow mFenetre;
	sf::Texture textureA, textureNiveau;
	std::vector< std::unique_ptr< sf::Sprite > > mVectSprite;
	sf::View mCamera;
	sf::VertexArray mVertArrayTileMap;
	TileMap tileMap;
	Moteur* mPtrMoteurPrincipal = nullptr;
public:
	MoteurGraphique() = default;
	void linkMainEngine( Moteur* ptrMoteur );
};

#endif // MOTEURGRAPHIQUE_H
