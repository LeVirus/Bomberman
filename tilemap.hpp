#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include "tableau2d.hpp"
#include <fstream>

class TileMap: public sf::Drawable, public sf::Transformable
{
private:
	sf::Texture mTexture;
	sf::VertexArray mVertArrayTileMap;
	std::vector< std::pair< unsigned int, unsigned int > > mvectPositionTuile;
	Tableau2D mTab;
	unsigned int muiLongueurTile = 0, muiLargeurTile = 0,
	muiLongueurNiveau = 0, muiLargeurNiveau = 0;

	void initialiserVertexArray();
	void setPositionPair( std::ifstream &flux );

	virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

public:
	TileMap() = default;
	bool loadTexture( const std::string &path );
	bool loadLevel( const std::string &path );
	//void setPositionPair( const std::vector< std::pair< unsigned int, unsigned int > > &vectPosTile );

	void configTileMap( unsigned int uiLongueurTile, unsigned int uiLargeurTile,
						unsigned int uiLongueurNiveau, unsigned int uiLargeurNiveau);
	const sf::VertexArray &getVertexArrayTileMap()const;
	const sf::Texture &getTextureTileMap()const;
	void displayTileMap()const;

	bool bDessinerVertArrayNiveau();
};

#endif // TILEMAP_H
