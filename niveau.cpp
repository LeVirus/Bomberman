#include "niveau.hpp"

Niveau::Niveau()
{

}

void Niveau::adaptToScale( float fX, float fY )
{
	muiLongueurCase *= fX;
	muiLargeurCase *= fY;
}

/*void TileMap::setPositionPair( const std::vector< std::pair< unsigned int, unsigned int > > &vectPosTile )
{
	mvectPositionTuile = vectPosTile;
}*/

void Niveau::setPositionPair( std::ifstream &flux )
{
	unsigned int uiNbrTuile;
	flux >> uiNbrTuile;
	mvectPositionTuile.resize( uiNbrTuile );
	for( unsigned int i = 0; i < mvectPositionTuile.size() ; ++i )
	{
		flux >> mvectPositionTuile[ i ].first;
		flux >> mvectPositionTuile[ i ].second;
	}
}

/**
 * @brief TileMap::loadLevel Chargement du niveau à partir du fichier texte de la classe Niveau.
 * structure du fichier::
 * chemin vers la texture du TileMap
 * muiLongueurNiveau
 * muiLargeurNiveau
 * muiLongueurTile
 * muiLargeurTile
 * nombre de tuile différentes
 * tableau de positions des tuiles dans la texture
 * tableau tilemap(Suite de nombre représentant le type de tuile pour chaque case)
 * fin fichier::
 * @param path Chemin vers le fichier de configuration.
 * @return
 */
bool Niveau::loadLevel( const std::string &path)
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
	assert(loadTexture( str ) && "erreur texture non chargé");
	if( ! loadTexture( str ) )
	{
		flux.close();
		return false;
	}
	flux >> muiLongueurNiveau;
	flux >> muiLargeurNiveau;
	flux >> muiLongueurTile;
	flux >> muiLargeurTile;

	setPositionPair( flux );
	//initialiserVertexArray();

	//si tout se passe correctement le flux est fermé dans la fonction bAttribuerTab.
	if( ! mtabNiveau.bAttribuerTab( flux, muiLongueurNiveau , muiLargeurNiveau ) )return false;

	//bDessinerVertArrayNiveau();
	return true;

}
