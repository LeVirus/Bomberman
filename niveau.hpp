#ifndef NIVEAU_HPP
#define NIVEAU_HPP

#include <vector>
#include <string>
#include <fstream>

typedef std::vector<std::pair<unsigned int, unsigned int>> vectPairUi_t;

struct TilemapBombermanComponent;

class Niveau
{
private:
	unsigned int muiLongueurTile, muiLargeurTile, muiLongueurNiveau, muiLargeurNiveau;
	vectPairUi_t mvectPositionTuile;
	std::string mPathToTexture;
    static unsigned int mNumEntityLevel;
public:
	Niveau();
	void adaptToScale( float fX, float fY );
    void setPositionPair(std::ifstream &flux , TilemapBombermanComponent &levelTileComp);
    bool loadLevel(unsigned int uiNumNiveau , unsigned int numEntityLevel, TilemapBombermanComponent &levelTileComp);
	unsigned int getLongueurTile()const;
	unsigned int getLargeurTile()const;
	unsigned int getLongueurNiveau()const;
	unsigned int getLargeurNiveau()const;
    const std::string &getPathToTexture()const;
	const vectPairUi_t &getVectPositionTile()const;
	void displayLevel()const;
    static unsigned int getNumEntityLevel();
};

#endif // NIVEAU_HPP
