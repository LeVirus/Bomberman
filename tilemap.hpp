#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include "tableau2d.hpp"
#include "niveau.hpp"
#include "tilemapbombermancomponent.hpp"
#include <fstream>

class TileMap: public sf::Drawable, public sf::Transformable
{
private:
    const sf::Texture *mTexture;
	sf::VertexArray mVertArrayTileMap;
	unsigned int muiLongueurTile = 0, muiLargeurTile = 0,
    muiLongueurMap = 0, muiLargeurMap = 0, muiNumEntity = 9000;

    void initialiserVertexArray(const TilemapBombermanComponent &tileComp);
	void setPositionPair( std::ifstream &flux );

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    TileMap() = default;
	bool loadLevel(const Niveau &level, unsigned int uiNumEntity );

	const sf::VertexArray &getVertexArrayTileMap()const;
	const sf::Texture &getTextureTileMap()const;
	void displayTileMap()const;

    void configureTileMap(const TilemapBombermanComponent &tileComp);
    bool bDessinerVertArrayNiveau(const TilemapBombermanComponent &tileComp);

	void adaptToScale( float fX, float fY );
	unsigned int getNumEntity()const{return muiNumEntity;}
    unsigned int getLongueurTile()const{return muiLongueurTile;}
	unsigned int getLargeurTile()const{return muiLargeurTile;}
};

#endif // TILEMAP_H
