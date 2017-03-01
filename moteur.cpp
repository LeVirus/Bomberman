#include "moteur.hpp"
#include "inputcomponent.hpp"
#include "inputsystem.hpp"
#include "ECSconstantes.hpp"
#include "componentmanager.hpp"
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

void Moteur::earnInput()
{
	/*const std::vector< unsigned int > &vectNumEntitySystem = mGestECS.getECSSystemManager() ->
			searchSystemByType< ecs::InputSystem > ( ecs::INPUT_SYSTEM )->getVectNumEntity();
	ecs::InputComponent ic = mGestECS.getECSComponentManager() ->
			searchComponentByType< ecs::InputComponent >( memEntity, ecs::INPUT_COMPONENT );
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ) )break;*/
}

GestionnaireECS &Moteur::getGestionnaireECS()
{
	return mGestECS;
}

void Moteur::loadTileMap( const std::string &pathTile )
{
	unsigned int memEntity;
	//création de l'entité avec les composants nécessaires
	std::vector< bool > bitsetComp;
	bitsetComp.resize( getGestionnaireECS().getECSComponentManager()->getNumberComponent() );
	bitsetComp[ ecs::DISPLAY_COMPONENT ] = true;
	bitsetComp[ ecs::POSITION_COMPONENT ] = true;
	memEntity = mGestECS.addEntity( bitsetComp );

	ecs::DisplayComponent * dc = mGestECS.getECSComponentManager() ->
			searchComponentByType< ecs::DisplayComponent >( memEntity, ecs::DISPLAY_COMPONENT );
	dc->muiNumSprite = SPRITE_TILEMAP;
	ecs::PositionComponent * pc = mGestECS.getECSComponentManager() ->
			searchComponentByType< ecs::PositionComponent >( memEntity, ecs::POSITION_COMPONENT );
	pc->vect2DPosComp.mfX = POSITION_LEVEL_X;
	pc->vect2DPosComp.mfY = POSITION_LEVEL_Y;
	//récupération et modification des composants
	mMoteurGraphique.loadTileMap( pathTile, memEntity );
}

bool Moteur::loadPlayersAndBot( unsigned int uiNumPlayer, unsigned int uiNumBot )
{
	if( MAX_PLAYER < uiNumPlayer + uiNumBot )return false;
	for( unsigned int i = 0 ; i < uiNumPlayer ; ++i )
	{
		std::vector< bool > bitsetComp;
		bitsetComp.resize( getGestionnaireECS().getECSComponentManager()->getNumberComponent() );
		bitsetComp[ ecs::DISPLAY_COMPONENT ] = true;
		bitsetComp[ ecs::POSITION_COMPONENT ] = true;
		bitsetComp[ ecs::MOVEABLE_COMPONENT ] = true;
		bitsetComp[ ecs::INPUT_COMPONENT ] = true;
		bitsetComp[ ecs::COLL_RECTBOX_COMPONENT ] = true;

		unsigned int memEntity = mGestECS.addEntity( bitsetComp );
		unsigned int memNumSprite = mMoteurGraphique.loadSprite(
					TEXTURE_BOMBERMAN, sf::IntRect( 71, 45, 16, 25 ) );

		ecs::DisplayComponent * dc = mGestECS.getECSComponentManager() ->
				searchComponentByType< ecs::DisplayComponent >( memEntity, ecs::DISPLAY_COMPONENT );
		dc->muiNumSprite = memNumSprite;

		mMoteurGraphique.positionnerCaseTileMap( memEntity, 1, 1 );

	}
	return true;
	//uiNumBot a implémenter ultérieurement
}

