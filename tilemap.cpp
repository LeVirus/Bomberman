#include "tilemap.hpp"
#include <fstream>
#include <iostream>
#include <cassert>

bool TileMap::loadTexture( const std::string &path )
{
	return mTexture.loadFromFile( path );
}

/**
 * @brief TileMap::loadLevel Chargement du niveau à partir du fichier texte de la classe Niveau.
 * structure du fichier::
 *
 * chemin vers la texture du TileMap
 * muiLongueurNiveau
 * muiLargeurNiveau
 * muiLongueurTile
 * muiLargeurTile
 * (Suite de nombre représentant le type de tuile pour chaque case)
 *
 * fin fichier::
 * @param path Chemin vers le fichier de configuration.
 * @return
 */
bool TileMap::loadLevel( const std::string &path )
{
	std::string str;
	std::ifstream flux( path, std::ios::in );
	assert( flux && "flux erreur" );
	if( !flux )
	{
		flux.close();
		return false;
	}

	flux >> str;
	if( ! loadTexture( str ) )
	{
		flux.close();
		return false;
	}
	flux >> muiLongueurNiveau;
	flux >> muiLargeurNiveau;
	flux >> muiLongueurTile;
	flux >> muiLargeurTile;

	initialiserVertexArray();

	//si tout se passe correctement le flux est fermé dans la fonction bAttribuerTab.
	if( ! mTab.bAttribuerTab( flux, muiLongueurTile , muiLargeurTile ) )return false;

	bDessinerVertArrayNiveau();
	return true;

}

void TileMap::configTileMap( unsigned int uiLongueurTile, unsigned int uiLargeurTile,
					unsigned int uiLongueurNiveau, unsigned int uiLargeurNiveau)
{
	muiLongueurTile = uiLongueurTile;
	muiLargeurTile = uiLargeurTile;
	muiLongueurNiveau = uiLongueurNiveau;
	muiLargeurNiveau = uiLargeurNiveau;
}

const sf::VertexArray &TileMap::getVertexArrayTileMap()const
{
	return mVertArrayTileMap;
}


void TileMap::initialiserVertexArray()
{

	unsigned int uiPosCaseX = 0, uiPosCaseY = 0;

	//détermination du type du tableau de vertex
	mVertArrayTileMap.setPrimitiveType ( sf::Quads );
	mVertArrayTileMap.resize ( muiLongueurNiveau * muiLargeurNiveau * 4 );

	//traitement de toutes les autres cases
	for( unsigned int i = 0 ; i < muiLongueurNiveau * muiLargeurNiveau * 4 ; i += 4 ){
		if( uiPosCaseX == muiLongueurNiveau )
		{
			uiPosCaseX = 0;
			uiPosCaseY++;
		}
		mVertArrayTileMap[ i ].position = sf::Vector2f( uiPosCaseX * muiLongueurTile, uiPosCaseY * muiLargeurTile );
		mVertArrayTileMap[ i + 1 ].position = sf::Vector2f( mVertArrayTileMap[ i ].position.x + muiLongueurTile,
															mVertArrayTileMap[ i ].position.y );
		mVertArrayTileMap[ i + 2 ].position = sf::Vector2f(
					mVertArrayTileMap[ i ].position.x + muiLongueurTile, mVertArrayTileMap[ i ].position.y + muiLargeurTile );
		mVertArrayTileMap[ i + 3 ].position = sf::Vector2f( mVertArrayTileMap[ i ].position.x,
															mVertArrayTileMap[ i ].position.y + muiLargeurTile );

		uiPosCaseX++;
	}
}

void TileMap::displayTileMap() const
{
	std::cout << "AFFICHAGE TILEMAP\n";
	std::cout << "muiLongueurNiveau::"<< muiLongueurNiveau <<
				 "\nmuiLargeurNiveau::"<< muiLargeurNiveau <<
				 "\nmuiLongueurTile::"<< muiLongueurTile <<
				 "\nmuiLargeurTile::"<< muiLargeurTile << "\n";
	mTab.afficherTab();
	std::cout << "FIN AFFICHAGE TILEMAP\n";


}

void TileMap::setPositionPair( const std::vector<std::pair<unsigned int, unsigned int> > &vectPosTile )
{
	mvectPositionTuile = vectPosTile;
}


/**
 * @brief Fonction qui dessine en fonction du tableau reçu de Niveau
 * La partie du niveau à afficher.
 * @param tabNivEcran Le tableau 2d de la partie du Niveau àœ
 * afficher.
 * @return si les valeurs de tabNivEcran sont valide
 * false sinon.
 */
bool TileMap::bDessinerVertArrayNiveau()
{
	bool retour = true;
	unsigned int uiCoordBlockX, uiCoordBlockY, uiCoordTabX = 0, uiCoordTabY = 0, uiValTile;
	for( unsigned int i = 0; i < mVertArrayTileMap.getVertexCount() ; i+=4 )
	{

		if( uiCoordTabX == muiLongueurNiveau ){
			uiCoordTabY++;
			uiCoordTabX = 0;
		}

		uiValTile = mTab.getValAt( uiCoordTabX, uiCoordTabY );
		uiCoordBlockX = mvectPositionTuile[ uiValTile ].first;
		uiCoordBlockY = mvectPositionTuile[ uiValTile ].second;

		if( ! retour )break;

		mVertArrayTileMap[ i ].texCoords = sf::Vector2f( uiCoordBlockX, uiCoordBlockY );
		mVertArrayTileMap[ i + 1 ].texCoords = sf::Vector2f( uiCoordBlockX + muiLongueurTile, uiCoordBlockY );
		mVertArrayTileMap[ i + 2 ].texCoords = sf::Vector2f( uiCoordBlockX + muiLongueurTile, uiCoordBlockY  + muiLargeurTile );
		mVertArrayTileMap[ i + 3 ].texCoords = sf::Vector2f( uiCoordBlockX, uiCoordBlockY + muiLargeurTile ) ;

		uiCoordTabX++;

	}
	return retour;
}
