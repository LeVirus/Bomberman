#ifndef NIVEAU_H
#define NIVEAU_H

#include <string>
#include <vector>

class Jeu;

class Niveau
{
private:
	Jeu* mJeu;
	unsigned int MUI_TOTAL_NIVEAU = 1;
	std::vector< std::string > mvectPathFileTileMap;
public:
	Niveau();
	void linkJeu( Jeu * jeu );
	const std::string &getConfLevelTileMap( unsigned int numNiv )const;
	void initListeNiveau();
};

#endif // NIVEAU_H
