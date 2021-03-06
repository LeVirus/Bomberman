#include "tilemap.hpp"
#include "moteurgraphique.hpp"
#include <iostream>
#include <cassert>
#include <string>

const sf::VertexArray &TileMap::getVertexArrayTileMap()const
{
	return mVertArrayTileMap;
}

const sf::Texture &TileMap::getTextureTileMap()const
{
    return *mTexture;
}

void TileMap::configureTileMap(const TilemapBombermanComponent &tileComp)
{
    const std::vector<sf::Texture> &tabText = MoteurGraphique::static_getTabTexture();
    assert(tileComp.mNumAssociateTexture < tabText.size() && "bad texture number");
    mTexture = &tabText[tileComp.mNumAssociateTexture];
    initialiserVertexArray(tileComp);
    bDessinerVertArrayNiveau(tileComp);
}

void TileMap::initialiserVertexArray(const TilemapBombermanComponent &tileComp)
{
	unsigned int uiPosCaseX = 0, uiPosCaseY = 0;

    muiLargeurTile = tileComp.mLenghtTile;
    muiHauteurTile = tileComp.mHeightTile;
    muiLongueurMap = tileComp.mTabTilemap.getLargeur();
    muiLargeurMap = tileComp.mTabTilemap.getHauteur();
    //détermination du type du tableau de vertex
	mVertArrayTileMap.setPrimitiveType ( sf::Quads );
    mVertArrayTileMap.resize(muiLongueurMap * muiLargeurMap * 4);
	//traitement de toutes les autres cases
    for(unsigned int i = 0 ; i < muiLongueurMap * muiLargeurMap * 4 ; i += 4)
    {
        if(uiPosCaseX == muiLongueurMap)
		{
			uiPosCaseX = 0;
			uiPosCaseY++;
		}
        mVertArrayTileMap[i].position = sf::Vector2f(uiPosCaseX * muiLargeurTile, uiPosCaseY * muiHauteurTile);
        mVertArrayTileMap[i + 1].position = sf::Vector2f(mVertArrayTileMap[i].position.x + muiLargeurTile,
                                                            mVertArrayTileMap[i].position.y);
        mVertArrayTileMap[i + 2].position = sf::Vector2f(
                    mVertArrayTileMap[i].position.x + muiLargeurTile, mVertArrayTileMap[i].position.y + muiHauteurTile);
        mVertArrayTileMap[i + 3].position = sf::Vector2f(mVertArrayTileMap[i].position.x,
                                                            mVertArrayTileMap[i].position.y + muiHauteurTile);

		uiPosCaseX++;
	}
}

/**
 * @brief Fonction qui dessine en fonction du tableau reçu de Niveau
 * La partie du niveau à afficher.
 * @param tabNivEcran Le tableau 2d de la partie du Niveau à
 * afficher.
 * @return si les valeurs de tabNivEcran sont valide
 * false sinon.
 */
bool TileMap::bDessinerVertArrayNiveau(const TilemapBombermanComponent &tileComp)
{
    bool retour = true;
    unsigned int uiCoordBlockX, uiCoordBlockY, uiCoordTabX = 0, uiCoordTabY = 0, uiValTile;
    const vectPairUi_t &vectPos = tileComp.mvectPositionTile;
    const Tableau2D &memTab = tileComp.mTabTilemap;
    for(unsigned int i = 0; i < mVertArrayTileMap.getVertexCount() ; i += 4)
    {
        if(uiCoordTabX == muiLongueurMap)
        {
            uiCoordTabY++;
            uiCoordTabX = 0;
        }

        uiValTile = memTab.getValAt(uiCoordTabX, uiCoordTabY);
        uiCoordBlockX = vectPos[uiValTile].first;
        uiCoordBlockY = vectPos[uiValTile].second;

        if(! retour)break;
        mVertArrayTileMap[i].texCoords = sf::Vector2f( uiCoordBlockX, uiCoordBlockY );
        mVertArrayTileMap[i + 1].texCoords = sf::Vector2f( uiCoordBlockX + muiLargeurTile, uiCoordBlockY );
        mVertArrayTileMap[i + 2].texCoords = sf::Vector2f( uiCoordBlockX + muiLargeurTile, uiCoordBlockY  + muiHauteurTile );
        mVertArrayTileMap[i + 3].texCoords = sf::Vector2f( uiCoordBlockX, uiCoordBlockY + muiHauteurTile ) ;
        uiCoordTabX++;
    }
    return retour;
}

void TileMap::displayTileMap() const
{
	std::cout << "AFFICHAGE TILEMAP\n";
    std::cout << "muiLongueurNiveau::"<< muiLongueurMap <<
                 "\nmuiLargeurNiveau::"<< muiLargeurMap <<
                 "\nmuiLongueurTile::"<< muiLargeurTile <<
                 "\nmuiLargeurTile::"<< muiHauteurTile << "\n";
	std::cout << "FIN AFFICHAGE TILEMAP\n";


}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	// on applique la transformation de l'entité -- on la combine avec celle qui a été passée par l'appelant
	states.transform *= getTransform(); // getTransform() est définie par sf::Transformable

	// on applique la texture
    states.texture = mTexture;
	// on peut aussi surcharger states.shader ou states.blendMode si nécessaire

	// on dessine le tableau de vertex
    target.draw(mVertArrayTileMap, states);
}

void TileMap::adaptToScale( float fX, float fY )
{
    muiLargeurTile *= fX;
    muiHauteurTile *= fY;
}
