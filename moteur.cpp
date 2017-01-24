#include "moteur.hpp"
#include "ECSconstantes.hpp"
#include "constants.hpp"
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
	ecs::DisplayComponent * dc = mGestECS.getECSComponentManager() ->
			searchComponentByType< ecs::DisplayComponent >( memEntity, ecs::DISPLAY_COMPONENT);
	dc->muiNumSprite = SPRITE_TILEMAP;
	ecs::PositionComponent * pc = mGestECS.getECSComponentManager() ->
			searchComponentByType< ecs::PositionComponent >( memEntity, ecs::POSITION_COMPONENT);
	pc->vect2DPosComp.mfX = 180;
	pc->vect2DPosComp.mfY = 70;
}

bool Moteur::loadPlayersAndBot( unsigned int uiNumPlayer, unsigned int uiNumBot )
{
	if( MAX_PLAYER < uiNumPlayer + uiNumBot )return false;
	for( unsigned int i ; i < uiNumPlayer ; ++i )
	{
		std::bitset< ecs::NUMBR_COMPONENT > bitsetComp;
		bitsetComp[ ecs::DISPLAY_COMPONENT ] = true;
		bitsetComp[ ecs::POSITION_COMPONENT ] = true;
		bitsetComp[ ecs::MOVEABLE_COMPONENT ] = true;
		bitsetComp[ ecs::INPUT_COMPONENT ] = true;
		bitsetComp[ ecs::COLL_RECTBOX_COMPONENT ] = true;

		memEntity = mGestECS.addEntity( bitsetComp );
	}
}

