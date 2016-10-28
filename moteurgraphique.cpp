#include "moteurgraphique.hpp"
#include "moteur.hpp"
#include <cassert>



void MoteurGraphique::initialiserFenetre()
{
	mFenetre.create(sf::VideoMode( 800, 600 ), "Bomber", sf::Style::Default );

	mCamera.reset( sf::FloatRect( 0, 0, 800 , 600 ) );
	mFenetre.setView( mCamera );
	mFenetre.setFramerateLimit(60);
}

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

void MoteurGraphique::raffraichirEcran()
{
	const sf::VertexArray vectTile = mTileMap.getVertexArrayTileMap();
	const sf::Texture text = mTileMap.getTextureTileMap();
	mFenetre.clear( sf::Color::Black );
	mFenetre.draw( vectTile , &text);
//	mFenetre.draw( vectTile, mTileMap.getTextureTileMap() );
	mFenetre.display();

}
