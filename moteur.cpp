#include "moteur.hpp"
#include "tilemap.hpp"
#include "vector2D.hpp"
#include "ECSconstantes.hpp"
#include "moveablebombermancomponent.hpp"
#include "constants.hpp"
#include "componentmanager.hpp"
#include "inputbombermancomponent.hpp"
#include "inputbombermansystem.hpp"
#include "collrectboxcomponent.hpp"
#include "flagcomponent.hpp"
#include "constants.hpp"
#include <SFML/Graphics.hpp>
#include <bitset>
#include <cassert>

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
		earnInput();

		mMoteurGraphique.raffraichirEcran();
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )break;
	}while( true );
}

void Moteur::earnInput()
{
	const std::vector< unsigned int > &vectNumEntitySystem = mGestECS.getECSSystemManager() ->
			searchSystemByType< InputBombermanSystem > ( INPUT_BOMBER_SYSTEM )->getVectNumEntity();


	for( unsigned int i = 0 ; i < vectNumEntitySystem.size() ; ++i )
	{

		InputBombermanComponent *ic = mGestECS.getECSComponentManager() ->
				searchComponentByType< InputBombermanComponent >(
					vectNumEntitySystem[i], BOMBER_INPUT_COMPONENT );
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ) )ic->mBitsetInput[MOVE_UP] = true;
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
		bitsetComp[ BOMBER_MOVEABLE_COMPONENT] = true;
		bitsetComp[ ecs::COLL_RECTBOX_COMPONENT ] = true;
		bitsetComp[ BOMBER_INPUT_COMPONENT ] = true;
		bitsetComp[ BOMBER_FLAG_COMPONENT ] = true;

		unsigned int memEntity = mGestECS.addEntity( bitsetComp );
		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<InputBombermanComponent>());

		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<MoveableBombermanComponent>());
		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<FlagBombermanComponent>());

		FlagBombermanComponent *fc = mGestECS.getECSComponentManager()->
				searchComponentByType < FlagBombermanComponent > ( memEntity, BOMBER_FLAG_COMPONENT );
		fc->muiNumFlag = FLAG_BOMBERMAN;

		ecs::CollRectBoxComponent * cc = mGestECS.getECSComponentManager() ->
				searchComponentByType< ecs::CollRectBoxComponent >( memEntity, ecs::COLL_RECTBOX_COMPONENT );
		cc->mRectBox.mSetHeightRectBox(20);
		cc->mRectBox.mSetLenghtRectBox(20);
		cc->mVect2dVectOrigins.mfX = 5;
		cc->mVect2dVectOrigins.mfY = 5;

		mMoteurGraphique.loadSprite(TEXTURE_BOMBERMAN, sf::IntRect( 71, 45, 16, 25 ) );

		mMoteurGraphique.positionnerCaseTileMap( memEntity, 1, 1 );

	}
	return true;
	//uiNumBot a implémenter ultérieurement
}

void Moteur::loadLevelWall()
{
	const TileMap &tilemap = mMoteurGraphique.getTileMap();
	unsigned int tailleNiveauX = tilemap.getLongueurNiveau(), tailleNiveauY = tilemap.getLargeurNiveau(),
			tailleTileX = tilemap.getLongueurTile(), tailleTileY = tilemap.getLargeurTile();


	std::vector< bool > bitsetComp;
	bitsetComp.resize( getGestionnaireECS().getECSComponentManager()->getNumberComponent() );
	bitsetComp[ ecs::POSITION_COMPONENT ] = true;
	bitsetComp[ ecs::COLL_RECTBOX_COMPONENT ] = true;
	bitsetComp[ BOMBER_FLAG_COMPONENT ] = true;

	unsigned int memEntity = mGestECS.addEntity( bitsetComp );


	mGestECS.getECSComponentManager()->
			instanciateExternComponent(memEntity, std::make_unique<FlagBombermanComponent>());

	FlagBombermanComponent *fc = mGestECS.getECSComponentManager()->
			searchComponentByType < FlagBombermanComponent > ( memEntity, BOMBER_FLAG_COMPONENT );
	fc->muiNumFlag = FLAG_SOLID_WALL;

	ecs::PositionComponent * pc = mGestECS.getECSComponentManager() ->
			searchComponentByType< ecs::PositionComponent >( memEntity, ecs::POSITION_COMPONENT );

	assert(pc && "Moteur::loadLevelWall positionComp == null\n");
	positionnerComponent(*pc, 9, 3, tailleTileX, tailleTileY);//test
	//pc->vect2DPosComp.mfX = POSITION_LEVEL_X + 1000;
	//pc->vect2DPosComp.mfY = POSITION_LEVEL_Y;
	ecs::CollRectBoxComponent * cc = mGestECS.getECSComponentManager() ->
			searchComponentByType< ecs::CollRectBoxComponent >( memEntity, ecs::COLL_RECTBOX_COMPONENT );
	cc->mRectBox.mSetHeightRectBox(20);
	cc->mRectBox.mSetLenghtRectBox(20);
	cc->mVect2dVectOrigins.mfX = 5;//positionner le décallage
	cc->mVect2dVectOrigins.mfY = 5;
	cc->mRectBox.mSetOriginsRectBox(ecs::Vector2D(POSITION_LEVEL_X, POSITION_LEVEL_Y));
}

const Niveau &Moteur::getNiveau() const
{
	return mNiveau;
}

void Moteur::positionnerComponent(ecs::PositionComponent &posComp, unsigned int posX, unsigned int posY,
								  unsigned int taileTileX, unsigned int taileTileY)
{
	/*std::cout << "POSITION_LEVEL_X " << POSITION_LEVEL_X<< "posX " <<  posX <<
				 "taileTileX " << taileTileX << "SIZE_SCALE " << SIZE_SCALE << " 48 "<<std::endl;*/
	posComp.vect2DPosComp.mfX = POSITION_LEVEL_X + posX * taileTileX;
	posComp.vect2DPosComp.mfY = POSITION_LEVEL_Y + posY * taileTileY;
}
