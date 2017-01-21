#include "moteur.hpp"
#include <SFML/Graphics.hpp>
#include <bitset>

Moteur::Moteur()
{
	mMoteurGraphique.linkMainEngine( this );
}

void Moteur::lancerBoucle()
{
	mMoteurGraphique.initialiserFenetre();
	do
	{
		mGestECS.getECSSystemManager()->bExecAllSystem();
		mMoteurGraphique.raffraichirEcran();
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )break;
	}while( true );
}

GestionnaireECS &Moteur::getGestionnaireECS()
{
	return mGestECS;
}


bool Moteur::loadTileMap( const std::string &pathTile )
{
	unsigned int memEntity;
	std::bitset< ecs::NUMBR_COMPONENT > bitsetComp;
	bitsetComp[ ecs::DISPLAY_COMPONENT ] = true;
	bitsetComp[ ecs::POSITION_COMPONENT ] = true;
	memEntity = mGestECS.addEntity( bitsetComp );
	mMoteurGraphique.loadTileMap( pathTile, memEntity );
}

