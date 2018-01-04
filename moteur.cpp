#include "moteur.hpp"
#include "tilemap.hpp"
#include "vector2D.hpp"
#include "ECSconstantes.hpp"
#include "moveablebombermancomponent.hpp"
#include "constants.hpp"
#include "componentmanager.hpp"
#include "inputbombermancomponent.hpp"
#include "tilemapbombermancomponent.hpp"
#include "tilemapsystem.hpp"
#include "inputbombermansystem.hpp"
#include "collrectboxcomponent.hpp"
#include "flagcomponent.hpp"
#include "playerconfigbombermancomponent.hpp"
#include "jeu.hpp"

#include "displaysystem.hpp"
#include "positioncomponent.hpp"

#include <SFML/Graphics.hpp>
#include <bitset>
#include <cassert>
#include <SFML/System/Clock.hpp>


Moteur::Moteur(const Jeu &jeu): mPtrJeu(jeu), mGestECS(*this)
{
	mMoteurGraphique.linkMainEngine( this );
}

const Jeu &Moteur::getJeu() const
{
	return mPtrJeu;
}

void Moteur::lancerBoucle()
{
	mMoteurGraphique.initialiserFenetre();

	do
	{
        mGestECS.getECSEngine()->execIteration();
		earnInput();
		mMoteurGraphique.raffraichirEcran();
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )break;
	}while( true );
}

void Moteur::earnInput()
{
	const std::vector< unsigned int > &vectNumEntitySystem = mGestECS.getECSSystemManager() ->
			searchSystemByType< InputBombermanSystem > ( INPUT_BOMBER_SYSTEM )->getVectNumEntity();

sf::Event event;
	for( unsigned int i = 0 ; i < vectNumEntitySystem.size() ; ++i )
	{

		InputBombermanComponent *ic = mGestECS.getECSComponentManager() ->
				searchComponentByType< InputBombermanComponent >(
					vectNumEntitySystem[i], BOMBER_INPUT_COMPONENT );
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))ic->mBitsetInput[MOVE_UP] = true;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))ic->mBitsetInput[MOVE_DOWN] = true;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))ic->mBitsetInput[MOVE_LEFT] = true;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))ic->mBitsetInput[MOVE_RIGHT] = true;
        if(! mLockLaunchBomb && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            ic->mBitsetInput[LAUNCH_BOMB] = true;
            mLockLaunchBomb = true;
        }

        if(mLockLaunchBomb)
        {
            mMoteurGraphique.getEventFromWindows(event);
            if(sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
            {
                mMoteurGraphique.getEventFromWindows(event);
                mLockLaunchBomb = false;
            }
        }
	}
}

GestionnaireECS &Moteur::getGestionnaireECS()
{
	return mGestECS;
}

void Moteur::loadLevelTileMap(Niveau &niv, unsigned int numNiv)
{
	unsigned int memEntity;
	//création de l'entité avec les composants nécessaires
    std::vector<bool> bitsetComp;
    bitsetComp.resize(getGestionnaireECS().getECSComponentManager()->getNumberComponent());
    bitsetComp[BOMBER_TILEMAP_COMPONENT] = true;
    bitsetComp[ecs::POSITION_COMPONENT] = true;
    memEntity = mGestECS.addEntity(bitsetComp);
    mGestECS.getECSComponentManager()->updateComponentFromEntity();

    mGestECS.getECSComponentManager()->instanciateExternComponent(memEntity, std::make_unique<TilemapBombermanComponent>());

    ecs::PositionComponent *pc = mGestECS.getECSComponentManager() ->
            searchComponentByType< ecs::PositionComponent >(memEntity, ecs::POSITION_COMPONENT);
    assert(pc && "Moteur::loadTileMap PositionComponent == null\n");
    pc->vect2DPosComp.mfX = POSITION_LEVEL_X;
    pc->vect2DPosComp.mfY = POSITION_LEVEL_Y;

    TilemapBombermanComponent *tmc = mGestECS.getECSComponentManager() ->
            searchComponentByType<TilemapBombermanComponent>(memEntity, BOMBER_TILEMAP_COMPONENT);
    assert(tmc && "Moteur::loadTileMap TilemapBombermanComponent == null\n");
    niv.loadLevel(numNiv, memEntity, *tmc);

    //récupération et modification des composants
    mMoteurGraphique.memorizeSizeTile(niv.getLongueurTile(), niv.getLargeurNiveau());

}

bool Moteur::loadPlayersAndBot(unsigned int uiNumPlayer, unsigned int uiNumBot)
{
    unsigned int memBombermanSprite;
    if(MAX_PLAYER < uiNumPlayer + uiNumBot)return false;
    memBombermanSprite = mMoteurGraphique.loadSprite(TEXTURE_BOMBERMAN, sf::IntRect(71, 45, 16, 25));
    mMoteurGraphique.loadSprite(TEXTURE_BOMBERMAN, sf::IntRect(210, 138, 10, 11));//TMP
    unsigned int largeurTile = mPtrJeu.getNiveau().getLargeurTile();
    unsigned int longueurTile = mPtrJeu.getNiveau().getLongueurTile();
    for(unsigned int i = 0 ; i < uiNumPlayer ; ++i)
	{
		std::vector< bool > bitsetComp;
		bitsetComp.resize( getGestionnaireECS().getECSComponentManager()->getNumberComponent() );
        bitsetComp[ecs::DISPLAY_COMPONENT]          = true;
        bitsetComp[ecs::POSITION_COMPONENT]         = true;
        bitsetComp[BOMBER_MOVEABLE_COMPONENT]       = true;
        bitsetComp[ecs::COLL_RECTBOX_COMPONENT]     = true;
        bitsetComp[BOMBER_INPUT_COMPONENT]          = true;
        bitsetComp[BOMBER_FLAG_COMPONENT]           = true;
        bitsetComp[BOMBER_PLAYER_CONFIG_COMPONENT]  = true;

        unsigned int memEntity = mGestECS.addEntity(bitsetComp);
		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<InputBombermanComponent>());

		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<MoveableBombermanComponent>());
		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<FlagBombermanComponent>());
        mGestECS.getECSComponentManager()->
                instanciateExternComponent(memEntity, std::make_unique<PlayerConfigBombermanComponent>());

        FlagBombermanComponent *fc = mGestECS.getECSComponentManager()->
                searchComponentByType <FlagBombermanComponent> (memEntity, BOMBER_FLAG_COMPONENT);
		fc->muiNumFlag = FLAG_BOMBERMAN;

		ecs::CollRectBoxComponent * cc = mGestECS.getECSComponentManager() ->
                searchComponentByType< ecs::CollRectBoxComponent >(memEntity, ecs::COLL_RECTBOX_COMPONENT);

        //offset
        cc->mVect2dVectOrigins.mfX = 5;
        cc->mRectBox.mSetLenghtRectBox(longueurTile - 10);
        cc->mVect2dVectOrigins.mfY = 36;
        cc->mRectBox.mSetHeightRectBox(largeurTile - 10);
        ecs::DisplayComponent * dc = mGestECS.getECSComponentManager() ->
                searchComponentByType< ecs::DisplayComponent >( memEntity, ecs::DISPLAY_COMPONENT );
        dc->muiNumSprite = memBombermanSprite;

        mMoteurGraphique.positionnerCaseTileMap(memEntity, 1, 1);

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

    unsigned int largeurTile = mPtrJeu.getNiveau().getLargeurTile();
    unsigned int longueurTile = mPtrJeu.getNiveau().getLongueurTile();
    unsigned int cmptX = 0, cmptY = 0;
	for(std::vector<unsigned char>::const_iterator it = memTabNiv.begin(); it != memTabNiv.end(); ++it)
	{
        if(*it != TILE_SOLID_WALL && *it != TILE_DESTRUCTIBLE_WALL)
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
    posComp.vect2DPosComp.mfX = POSITION_LEVEL_X + posX * mPtrJeu.getNiveau().getLongueurTile();
    posComp.vect2DPosComp.mfY = POSITION_LEVEL_Y + posY * mPtrJeu.getNiveau().getLargeurTile();
//	std::cout << "XX " << posX << " posComp.X " <<posComp.vect2DPosComp.mfX
//			  << " YY " << posY << " posComp.Y " <<posComp.vect2DPosComp.mfY<< std::endl;
}
