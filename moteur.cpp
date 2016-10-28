#include "moteur.hpp"
#include <SFML/Graphics.hpp>

Moteur::Moteur()
{
	mMoteurGraphique.linkMainEngine( this );
}

void Moteur::lancerBoucle()
{
	mMoteurGraphique.initialiserFenetre();
	do
	{
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )break;
		mMoteurGraphique.raffraichirEcran();
	}while( true );
}

bool Moteur::loadTileMap( const std::string &pathTile )
{
	mMoteurGraphique.loadTileMap( pathTile );
}

