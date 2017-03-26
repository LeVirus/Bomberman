#ifndef NIVEAU_HPP
#define NIVEAU_HPP

#include <tableau2d.hpp>

class Niveau
{
private:
	Tableau2D mtabNiveau;
	unsigned int muiLongueurCase, muiLargeurCase;
public:
	Niveau();
	void adaptToScale( float fX, float fY );
	void setPositionPair( std::ifstream &flux );
	bool loadLevel(const std::string &path );
};

#endif // NIVEAU_HPP
