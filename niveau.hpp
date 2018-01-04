#ifndef NIVEAU_HPP
#define NIVEAU_HPP

#include <tableau2d.hpp>

typedef std::vector<std::pair<unsigned int, unsigned int>> vectPairUi_t;

struct TilemapBombermanComponent;

class Niveau
{
private:
    static Tableau2D mtabNiveau;
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
    bool destructWall(unsigned int x, unsigned int y);
    static unsigned int getNumEntityLevel();
    static const Tableau2D &getTabNiveau();
};

#endif // NIVEAU_HPP
