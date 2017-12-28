#ifndef NIVEAU_HPP
#define NIVEAU_HPP

#include <tableau2d.hpp>

typedef std::vector<std::pair<unsigned int, unsigned int>> vectPairUi_t;

class Niveau
{
private:
    static Tableau2D mtabNiveau;
	unsigned int muiLongueurTile, muiLargeurTile, muiLongueurNiveau, muiLargeurNiveau;
	vectPairUi_t mvectPositionTuile;
	std::string mPathToTexture;
public:
	Niveau();
	void adaptToScale( float fX, float fY );
	void setPositionPair( std::ifstream &flux );
	bool loadLevel(unsigned int uiNumNiveau );
    static const Tableau2D &getTabNiveau();
	unsigned int getLongueurTile()const;
	unsigned int getLargeurTile()const;
	unsigned int getLongueurNiveau()const;
	unsigned int getLargeurNiveau()const;
	const std::string &getPathToTexture()const;
	const vectPairUi_t &getVectPositionTile()const;
	void displayLevel()const;
    bool destructWall(unsigned int x, unsigned int y);
};

#endif // NIVEAU_HPP
