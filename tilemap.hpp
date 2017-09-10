#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include "tableau2d.hpp"
#include "niveau.hpp"
#include <fstream>

class TileMap: public sf::Drawable, public sf::Transformable
{
private:
	sf::Texture mTexture;
	sf::VertexArray mVertArrayTileMap;
	//std::vector< std::pair< unsigned int, unsigned int > > mvectPositionTuile;
	//Tableau2D mTab;
	unsigned int muiLongueurTile = 0, muiLargeurTile = 0,
	muiLongueurNiveau = 0, muiLargeurNiveau = 0, muiNumEntity = 9000;

	void initialiserVertexArray();
	void setPositionPair( std::ifstream &flux );

	virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

public:
	TileMap() = default;
	bool loadTexture( const std::string &path );
	bool loadLevel(const Niveau &level, unsigned int uiNumEntity );
	//void setPositionPair( const std::vector< std::pair< unsigned int, unsigned int > > &vectPosTile );

	void configTileMap(const Niveau &niveau);
	const sf::VertexArray &getVertexArrayTileMap()const;
	//const Tableau2D &getTabLevel()const;
	const sf::Texture &getTextureTileMap()const;
	void displayTileMap()const;

	bool bDessinerVertArrayNiveau(const Niveau &niv);

	void adaptToScale( float fX, float fY );
	unsigned int getNumEntity()const{return muiNumEntity;}
    unsigned int getLongueurTile()const{return muiLongueurTile;}
	unsigned int getLargeurTile()const{return muiLargeurTile;}
/*	unsigned int getLongueurNiveau()const{return muiLongueurNiveau;}
	unsigned int getLargeurNiveau()const{return muiLargeurNiveau;}*/
};

#endif // TILEMAP_H
