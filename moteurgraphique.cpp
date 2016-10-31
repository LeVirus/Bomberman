#include "moteurgraphique.hpp"
#include "moteur.hpp"
#include <cassert>



void MoteurGraphique::initialiserFenetre()
{
	mFenetre.create(sf::VideoMode( 800, 600 ), "Bomber", sf::Style::Fullscreen );

	mCamera.reset( sf::FloatRect( 0, 0, 800 , 600 ) );
	mFenetre.setView( mCamera );
	mFenetre.setFramerateLimit(60);
}

void MoteurGraphique::linkMainEngine( Moteur* ptrMoteur )
{
	assert( ptrMoteur && "ptrMoteur == null\n" );
	mPtrMoteurPrincipal = ptrMoteur;
}

bool MoteurGraphique::loadTileMap(const std::string &strPathConfFile , unsigned int uiNumEntity )
{
	mTileMap.loadLevel( strPathConfFile, uiNumEntity );
	mTileMap.displayTileMap();
}

void MoteurGraphique::raffraichirEcran()
{
	mFenetre.clear( sf::Color::Black );
	mTileMap.setScale( 3.0f, 3.0f );
	mTileMap.setPosition( 0.0f, 0.0f );
	mFenetre.draw( mTileMap );
//	mFenetre.draw( vectTile, mTileMap.getTextureTileMap() );
	mFenetre.display();

}
