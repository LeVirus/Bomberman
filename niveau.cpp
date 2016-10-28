#include "niveau.hpp"
#include "jeu.hpp"

Niveau::Niveau()
{
	initListeNiveau();
}

const std::string &Niveau::getConfLevelTileMap( unsigned int numNiv )const
{
	return mvectPathFileTileMap[ numNiv ];
}

void Niveau::linkJeu( Jeu * jeu )
{
	mJeu = jeu;
}

void Niveau::initListeNiveau()
{
	mvectPathFileTileMap.resize( MUI_TOTAL_NIVEAU );
	mvectPathFileTileMap[ 0 ] = "../Ressources/Niveau/Niveau1TM";
}
