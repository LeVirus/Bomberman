#include "moteur.hpp"

Moteur::Moteur()
{
	mMoteurGraphique.linkMainEngine( this );
}

bool Moteur::loadTileMap( const std::string &pathTile )
{
	mMoteurGraphique.loadTileMap( pathTile );
}

