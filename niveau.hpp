#ifndef NIVEAU_HPP
#define NIVEAU_HPP

#include <vector>
#include <string>
#include <fstream>
#include "tableau2d.hpp"

typedef std::vector<std::pair<unsigned int, unsigned int>> vectPairUi_t;

struct TilemapBombermanComponent;

class Niveau
{
private:
	unsigned int muiLongueurTile, muiLargeurTile, muiLongueurNiveau, muiLargeurNiveau;
	std::string mPathToTexture;
    static unsigned int mNumEntityLevel, mMaxPlayer, mCurrentNumberPlayer;
    static vectPairUi_t mInitBombermanPosition;
    static Tableau2D mTabPositionDestructWall;

    void setPositionPair(std::ifstream &flux , TilemapBombermanComponent &levelTileComp);
    bool setInitPositionBomberman(std::ifstream &flux);

public:
	Niveau();
    void displayLevel()const;
    bool loadLevel(unsigned int uiNumNiveau , unsigned int numEntityLevel, TilemapBombermanComponent &levelTileComp);
    void adaptToScale(float fX, float fY);
    unsigned int getLongueurTile()const;
    unsigned int getLargeurTile()const;
    unsigned int getLongueurNiveau()const;
    unsigned int getLargeurNiveau()const;
    const std::string &getPathToTexture()const;

    static unsigned int getNumCurrentNumberPlayer();
    static unsigned int getNumEntityLevel();
    static unsigned char static_getNumWallEntityOnPosition(unsigned int x, unsigned int y);
    static bool static_setNumWallEntityOnPosition(unsigned int x, unsigned int y, unsigned char val);
    static const vectPairUi_t &static_getVectInitPositionBomberman();
    static void decrementCurrentNumberPlayers();
};

#endif // NIVEAU_HPP
