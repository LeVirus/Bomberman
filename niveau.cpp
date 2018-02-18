#include "niveau.hpp"
#include "tilemapbombermancomponent.hpp"
#include <cassert>
#include <iostream>

unsigned int Niveau::mNumEntityLevel;
Tableau2D Niveau::mTabPositionDestructWall;
vectPairUi_t Niveau::mInitBombermanPosition;
unsigned int Niveau::mMaxPlayer;
unsigned int Niveau::mCurrentNumberPlayer;

Niveau::Niveau()
{

}

void Niveau::adaptToScale(float fX, float fY)
{
    muiLargeurTile *= fY;
    muiLongueurTile *= fX;
}

void Niveau::setPositionPair(TilemapBombermanComponent &levelTileComp, unsigned int numLevel)
{
    switch(numLevel)
    {
        case 0:
        levelTileComp.mvectPositionTile.push_back({17, 0});
        levelTileComp.mvectPositionTile.push_back({52, 0});
        levelTileComp.mvectPositionTile.push_back({34, 0});
        levelTileComp.mvectPositionTile.push_back({68, 0});
        levelTileComp.mvectPositionTile.push_back({0, 17});
        levelTileComp.mvectPositionTile.push_back({17, 17});
        levelTileComp.mvectPositionTile.push_back({34, 17});
        levelTileComp.mvectPositionTile.push_back({68, 17});
        levelTileComp.mvectPositionTile.push_back({85, 17});
        levelTileComp.mvectPositionTile.push_back({102, 17});
        break;
    }

//	unsigned int uiNbrTuile;
//	flux >> uiNbrTuile;
//    levelTileComp.mvectPositionTile.resize(uiNbrTuile);
//    for(unsigned int i = 0; i < levelTileComp.mvectPositionTile.size() ; ++i)
//	{
//        flux >> levelTileComp.mvectPositionTile[i].first;
//        flux >> levelTileComp.mvectPositionTile[i].second;
//    }
}

bool Niveau::setInitPositionBomberman(std::ifstream &flux)
{
    flux >> mMaxPlayer;
    mCurrentNumberPlayer = mMaxPlayer;
    if(! mMaxPlayer || mMaxPlayer > 6)
    {
        flux.close();
        return false;
    }
    mInitBombermanPosition.resize(mMaxPlayer);
    for(unsigned int i = 0; i < mMaxPlayer; ++i)
    {
        flux >> mInitBombermanPosition[i].first;
        flux >> mInitBombermanPosition[i].second;
    }
    return true;
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
bool Niveau::loadLevel(unsigned int uiNumNiveau, unsigned int numEntityLevel, TilemapBombermanComponent &levelTileComp)
{
	std::string path;
	switch (uiNumNiveau) {
	case 0:
        path = "../Bomberman/Ressources/Niveau/Niveau1TM";
		break;
	default:
		break;
	}
    std::ifstream flux(path, std::ios::in);
    assert(flux && "flux erreur");
	if( !flux )
	{
		flux.close();
		return false;
	}

    mNumEntityLevel = numEntityLevel;
	flux >> mPathToTexture;

	flux >> muiLongueurNiveau;
	flux >> muiLargeurNiveau;
	flux >> muiLongueurTile;
	flux >> muiLargeurTile;

    levelTileComp.mNumAssociateTexture = TEXTURE_LEVEL;

    mTabPositionDestructWall.resize(muiLongueurNiveau, muiLargeurNiveau);

    levelTileComp.mTabTilemap.resize(muiLongueurNiveau, muiLargeurNiveau);
    levelTileComp.mPersistant = true;
    levelTileComp.mHeightTile = muiLargeurTile;
    levelTileComp.mLenghtTile = muiLongueurTile;
    setPositionPair(levelTileComp, uiNumNiveau);

    if(! setInitPositionBomberman(flux))
    {
        return false;
    }
	//si tout se passe correctement le flux est fermé dans la fonction bAttribuerTab.
    if(! levelTileComp.mTabTilemap.bAttribuerTab(flux, muiLongueurNiveau , muiLargeurNiveau))return false;
    return true;
}

unsigned int Niveau::getLongueurNiveau() const
{
	return muiLongueurNiveau;
}

unsigned int Niveau::getLargeurNiveau() const
{
    return muiLargeurNiveau;
}

unsigned int Niveau::getNumEntityLevel()
{
    return mNumEntityLevel;
}

unsigned char Niveau::static_getNumWallEntityOnPosition(unsigned int x, unsigned int y)
{
    unsigned char res = mTabPositionDestructWall.getValAt(x, y);
    assert(res != 254 && "Bad value for tableau2d level");
    return res;
}

bool Niveau::static_setNumWallEntityOnPosition(unsigned int x, unsigned int y, unsigned char val)
{
    return mTabPositionDestructWall.setValAt(x, y, val);
}

const vectPairUi_t &Niveau::static_getVectInitPositionBomberman()
{
    return mInitBombermanPosition;
}

void Niveau::decrementCurrentNumberPlayers()
{
    --mCurrentNumberPlayer;
}

const std::__cxx11::string &Niveau::getPathToTexture() const
{
	return mPathToTexture;
}

void Niveau::displayLevel() const
{
	std::cout << "AFFICHAGE TILEMAP\n";
	std::cout << "muiLongueurNiveau::"<< muiLongueurNiveau <<
				 "\nmuiLargeurNiveau::"<< muiLargeurNiveau <<
				 "\nmuiLongueurTile::"<< muiLongueurTile <<
				 "\nmuiLargeurTile::"<< muiLargeurTile << "\n";
    std::cout << "FIN AFFICHAGE TILEMAP\n";
}

unsigned int Niveau::getNumCurrentNumberPlayer()
{
    return mCurrentNumberPlayer;
}

unsigned int Niveau::getLargeurTile() const
{
	return muiLargeurTile;
}

unsigned int Niveau::getLongueurTile()const
{
	return muiLongueurTile;
}
