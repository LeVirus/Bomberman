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
#include "jeu.hpp"
#include <SFML/Graphics.hpp>
#include <bitset>
#include <cassert>
#include <SFML/System/Clock.hpp>
Moteur::Moteur(const Jeu &jeu): mPtrJeu(jeu)
{
	mMoteurGraphique.linkMainEngine( this );
}

/*void Moteur::linkJeu(Jeu &jeu)
{
	mPtrJeu = &jeu;
}*/

const Jeu &Moteur::getJeu() const
{
	return mPtrJeu;
}

void Moteur::lancerBoucle()
{
//    sf::Clock clock;
	mMoteurGraphique.initialiserFenetre();
	do
	{
        clock.restart();
		mGestECS.getECSSystemManager()->bExecAllSystem();
		earnInput();
		mMoteurGraphique.raffraichirEcran();
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )break;
//       float time = clock.getElapsedTime().asMilliseconds ();
//       std::cout << "Time elapsed :: " << time << "\n";
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

void Moteur::loadTileMap(const Niveau &niv)
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
	mMoteurGraphique.loadTileMap( niv, memEntity );
}

bool Moteur::loadPlayersAndBot( unsigned int uiNumPlayer, unsigned int uiNumBot )
{
    unsigned int memBombermanSprite;
	if( MAX_PLAYER < uiNumPlayer + uiNumBot )return false;
    memBombermanSprite = mMoteurGraphique.loadSprite(TEXTURE_BOMBERMAN, sf::IntRect( 71, 45, 16, 25 ) );
    unsigned int largeurTile = mMoteurGraphique.getTileMap().getLargeurTile();
    unsigned int longueurTile = mMoteurGraphique.getTileMap().getLongueurTile();
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
        std::cout << "largeurTile :: " << largeurTile << "  longueurTile :: " << longueurTile << std::endl;


        cc->mVect2dVectOrigins.mfX = 5;
        cc->mRectBox.mSetLenghtRectBox(longueurTile - 10);

        cc->mVect2dVectOrigins.mfY = 36;
        cc->mRectBox.mSetHeightRectBox(largeurTile - 10);


        ecs::DisplayComponent * dc = mGestECS.getECSComponentManager() ->
                searchComponentByType< ecs::DisplayComponent >( memEntity, ecs::DISPLAY_COMPONENT );
        dc->muiNumSprite = memBombermanSprite;

        mMoteurGraphique.positionnerCaseTileMap( memEntity, 1, 1 );

	}
	return true;
	//uiNumBot a implémenter ultérieurement
}

void Moteur::loadLevelWall(const Niveau &niv)
{
	std::vector< bool > bitsetComp(getGestionnaireECS().getECSComponentManager()->getNumberComponent());
	const std::vector<unsigned char> &memTabNiv = niv.getTabNiveau().getTab();
	unsigned int longueurNiveau = niv.getTabNiveau().getLongueur();
	//bitsetComp.resize( getGestionnaireECS().getECSComponentManager()->getNumberComponent() );

    unsigned int largeurTile = mMoteurGraphique.getTileMap().getLargeurTile();
    unsigned int longueurTile = mMoteurGraphique.getTileMap().getLongueurTile();
    unsigned int cmptX = 0, cmptY = 0;
	for(std::vector<unsigned char>::const_iterator it = memTabNiv.begin(); it != memTabNiv.end(); ++it)
	{
		if(*it != FLAG_SOLID_WALL && *it != FLAG_DESTRUCTIBLE_WALL)
		{
			++cmptX;
			if(cmptX >= longueurNiveau)
			{
				cmptX = 0;
				++cmptY;
			}
			continue;
		}
		bitsetComp.assign(bitsetComp.size(), false);

		bitsetComp[ ecs::POSITION_COMPONENT ] = true;
		bitsetComp[ ecs::COLL_RECTBOX_COMPONENT ] = true;
		bitsetComp[ BOMBER_FLAG_COMPONENT ] = true;

		unsigned int memEntity = mGestECS.addEntity( bitsetComp );

		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<FlagBombermanComponent>());

		FlagBombermanComponent *fc = mGestECS.getECSComponentManager()->
				searchComponentByType < FlagBombermanComponent > ( memEntity, BOMBER_FLAG_COMPONENT );
		fc->muiNumFlag = *it;

		ecs::PositionComponent * pc = mGestECS.getECSComponentManager() ->
				searchComponentByType< ecs::PositionComponent >( memEntity, ecs::POSITION_COMPONENT );

		assert(pc && "Moteur::loadLevelWall positionComp == null\n");
		ecs::CollRectBoxComponent * cc = mGestECS.getECSComponentManager() ->
				searchComponentByType< ecs::CollRectBoxComponent >( memEntity, ecs::COLL_RECTBOX_COMPONENT );
        cc->mRectBox.mSetHeightRectBox(largeurTile);
        cc->mRectBox.mSetLenghtRectBox(longueurTile);
		//positionner le décallage
        cc->mVect2dVectOrigins.mfX = 0;
        cc->mVect2dVectOrigins.mfY = 0;
        positionnerComponent(*pc, cmptX, cmptY);

        cc->mRectBox.mSetOriginsRectBox(pc->vect2DPosComp + cc->mVect2dVectOrigins);
		++cmptX;
		if(cmptX >= longueurNiveau)
		{
			cmptX = 0;
			++cmptY;
		}
	}
}

void Moteur::positionnerComponent(ecs::PositionComponent &posComp, unsigned int posX, unsigned int posY)
{
    posComp.vect2DPosComp.mfX = POSITION_LEVEL_X + posX * mMoteurGraphique.getTileMap().getLongueurTile();
    posComp.vect2DPosComp.mfY = POSITION_LEVEL_Y + posY * mMoteurGraphique.getTileMap().getLargeurTile();
	std::cout << "XX " << posX << " posComp.X " <<posComp.vect2DPosComp.mfX
			  << " YY " << posY << " posComp.Y " <<posComp.vect2DPosComp.mfY<< std::endl;
}
