#ifndef NIVEAU_HPP
#define NIVEAU_HPP


#include <tableau2d.hpp>

typedef std::vector<std::pair<unsigned int, unsigned int>> vectPairUi_t;

class Niveau
{
private:
	Tableau2D mtabNiveau;
	unsigned int muiLongueurTile, muiLargeurTile, muiLongueurNiveau, muiLargeurNiveau;
	vectPairUi_t mvectPositionTuile;
	std::string mPathToTexture;
public:
	Niveau();
	void adaptToScale( float fX, float fY );
	void setPositionPair( std::ifstream &flux );
	bool loadLevel(unsigned int uiNumNiveau );
	const Tableau2D &getTabNiveau()const;
	unsigned int getLongueurTile()const;
	unsigned int getLargeurTile()const;
	unsigned int getLongueurNiveau()const;
	unsigned int getLargeurNiveau()const;
	const std::string &getPathToTexture()const;
	const vectPairUi_t &getVectPositionTile()const;
	void displayLevel()const;
};

#endif // NIVEAU_HPP
