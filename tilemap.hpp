#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include "tableau2d.hpp"

class TileMap
{
private:
	sf::Sprite mSpriteTileMap;
	sf::Texture mTexture;
	sf::VertexArray mVertArrayTileMap;
	std::vector< std::pair< unsigned int, unsigned int > > mvectPositionTuile;
	Tableau2D mTab;
	unsigned int muiLongueurTile = 0, muiLargeurTile = 0,
	muiLongueurNiveau = 0, muiLargeurNiveau = 0;
public:
	TileMap() = default;
	bool loadTexture( std::string path );
	bool loadLevel( std::string path );
	void setPositionPair( const std::vector< std::pair< unsigned int, unsigned int > > &vectPosTile );
	void configTileMap( unsigned int uiLongueurTile, unsigned int uiLargeurTile,
						unsigned int uiLongueurNiveau, unsigned int uiLargeurNiveau);
	const sf::Sprite &getSpriteTileMap()const;
	void initialiserVertexArray();

	bool bDessinerVertArrayNiveau();
};

#endif // TILEMAP_H
