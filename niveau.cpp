#include "niveau.hpp"
#include <cassert>
#include <iostream>

Tableau2D Niveau::mtabNiveau;

Niveau::Niveau()
{

}

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
bool Niveau::loadLevel( unsigned int uiNumNiveau)
{
	std::string path;
	switch (uiNumNiveau) {
	case 0:
        path = "../Bomberman/Ressources/Niveau/Niveau1TM";
		break;
	default:
		break;
	}
	std::ifstream flux( path, std::ios::in );
	assert( flux && "flux erreur" );
	if( !flux )
	{
		flux.close();
		return false;
	}

	flux >> mPathToTexture;

	flux >> muiLongueurNiveau;
	flux >> muiLargeurNiveau;
	flux >> muiLongueurTile;
	flux >> muiLargeurTile;

	setPositionPair( flux );

	//si tout se passe correctement le flux est fermé dans la fonction bAttribuerTab.
	if( ! mtabNiveau.bAttribuerTab( flux, muiLongueurNiveau , muiLargeurNiveau ) )return false;
	return true;

}

const Tableau2D &Niveau::getTabNiveau() const
{
	return mtabNiveau;
}

unsigned int Niveau::getLongueurNiveau() const
{
	return muiLongueurNiveau;
}

unsigned int Niveau::getLargeurNiveau() const
{
	return muiLargeurNiveau;
}

const std::__cxx11::string &Niveau::getPathToTexture() const
{
	return mPathToTexture;
}

const vectPairUi_t &Niveau::getVectPositionTile() const
{
	return mvectPositionTuile;
}

void Niveau::displayLevel() const
{
	std::cout << "AFFICHAGE TILEMAP\n";
	std::cout << "muiLongueurNiveau::"<< muiLongueurNiveau <<
				 "\nmuiLargeurNiveau::"<< muiLargeurNiveau <<
				 "\nmuiLongueurTile::"<< muiLongueurTile <<
				 "\nmuiLargeurTile::"<< muiLargeurTile << "\n";
	for( auto i : mvectPositionTuile )
	{
		std::cout << "tuile x::" << i.first <<"tuile y::" << i.second <<"\n" ;
	}
	mtabNiveau.afficherTab();
	std::cout << "FIN AFFICHAGE TILEMAP\n";
}

unsigned int Niveau::getLargeurTile() const
{
	return muiLargeurTile;
}

unsigned int Niveau::getLongueurTile()const
{
	return muiLongueurTile;
}
