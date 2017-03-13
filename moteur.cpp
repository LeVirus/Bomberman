#include "moteur.hpp"
#include "ECSconstantes.hpp"
#include "constants.hpp"
#include "componentmanager.hpp"
#include "inputbombermancomponent.hpp"
#include "inputbombermansystem.hpp"
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
		std::cout<< "in\n";
		earnInput();
		std::cout<< "out\n";

		mMoteurGraphique.raffraichirEcran();
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )break;
	}while( true );
}

void Moteur::earnInput()
{
	const std::vector< unsigned int > &vectNumEntitySystem = mGestECS.getECSSystemManager() ->
			searchSystemByType< InputBombermanSystem > ( INPUT_BOMBER_SYSTEM )->getVectNumEntity();

	std::cout <<"ss\n";

	for( unsigned int i = 0 ; i < vectNumEntitySystem.size() ; ++i )
	{
		std::cout <<"qq\n";

		InputBombermanComponent *ic = mGestECS.getECSComponentManager() ->
				searchComponentByType< InputBombermanComponent >(
					vectNumEntitySystem[i], BOMBER_INPUT_COMPONENT );
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ) )
		{
			std::cout <<"haut\n";
			ic->mBitsetInput[MOVE_UP] = true;
		}
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )ic->mBitsetInput[MOVE_DOWN] = true;
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Q ) )ic->mBitsetInput[MOVE_LEFT] = true;
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )ic->mBitsetInput[MOVE_RIGHT] = true;
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) )ic->mBitsetInput[LAUNCH_BOMB] = true;
	}
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
		bitsetComp[ ecs::COLL_RECTBOX_COMPONENT ] = true;
		bitsetComp[ BOMBER_INPUT_COMPONENT ] = true;

		unsigned int memEntity = mGestECS.addEntity( bitsetComp );
		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<InputBombermanComponent>());

		unsigned int memNumSprite = mMoteurGraphique.loadSprite(
					TEXTURE_BOMBERMAN, sf::IntRect( 71, 45, 16, 25 ) );

		ecs::DisplayComponent * dc = mGestECS.getECSComponentManager() ->
				searchComponentByType< ecs::DisplayComponent >( memEntity, ecs::DISPLAY_COMPONENT );
		dc->muiNumSprite = memNumSprite;

		InputBombermanComponent * ib = mGestECS.getECSComponentManager() ->
				searchComponentByType< InputBombermanComponent >( memEntity, BOMBER_INPUT_COMPONENT );
		std::cerr<<ib->muiGetIdComponent()<<"\n";

		mMoteurGraphique.positionnerCaseTileMap( memEntity, 1, 1 );

	}
	return true;
	//uiNumBot a implémenter ultérieurement
}

