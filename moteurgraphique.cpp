#include "moteurgraphique.hpp"
#include "moteur.hpp"
#include <cassert>

void MoteurGraphique::linkMainEngine( Moteur* ptrMoteur )
{
	assert( ptrMoteur && "ptrMoteur == null\n" );
	mPtrMoteurPrincipal = ptrMoteur;
}

bool MoteurGraphique::loadTileMap( const std::string &strPathConfFile )
{
	mTileMap.loadLevel( strPathConfFile );
	mTileMap.displayTileMap();
}
