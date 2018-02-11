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
#include "timerbombermancomponent.hpp"

#include <SFML/Graphics.hpp>
#include <bitset>
#include <cassert>
#include <SFML/System/Clock.hpp>
#include <iostream>


Moteur::Moteur(const Jeu &jeu):  mGestECS(*this), mPtrJeu(jeu)
{
    mMoteurGraphique.linkMainEngine(this);
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
        getInput();
		mMoteurGraphique.raffraichirEcran();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            mLetMeOut = true;
            break;
        }
        if(Niveau::getNumCurrentNumberPlayer() <= 1)
        {
            mGestECS.getECSEngine()->RmAllEntity();
            break;
        }
    }while(true);

}

void Moteur::getInput()
{
    InputBombermanSystem * ss = mGestECS.getECSSystemManager() ->
                searchSystemByType< InputBombermanSystem > ( INPUT_BOMBER_SYSTEM );
    assert(ss && "sdfs");
	const std::vector< unsigned int > &vectNumEntitySystem = mGestECS.getECSSystemManager() ->
			searchSystemByType< InputBombermanSystem > ( INPUT_BOMBER_SYSTEM )->getVectNumEntity();

	for( unsigned int i = 0 ; i < vectNumEntitySystem.size() ; ++i )
	{

		InputBombermanComponent *ic = mGestECS.getECSComponentManager() ->
				searchComponentByType< InputBombermanComponent >(
                    vectNumEntitySystem[i], INPUT_BOMBER_COMPONENT );
        if(! ic)continue;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))ic->mBitsetInput[MOVE_UP] = true;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))ic->mBitsetInput[MOVE_DOWN] = true;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))ic->mBitsetInput[MOVE_LEFT] = true;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))ic->mBitsetInput[MOVE_RIGHT] = true;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            ic->mBitsetInput[LAUNCH_BOMB] = true;
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
    bitsetComp[TILEMAP_BOMBER_COMPONENT] = true;
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
            searchComponentByType<TilemapBombermanComponent>(memEntity, TILEMAP_BOMBER_COMPONENT);
    assert(tmc && "Moteur::loadTileMap TilemapBombermanComponent == null\n");
    niv.loadLevel(numNiv, memEntity, *tmc);

    niv.adaptToScale(SIZE_SCALE, SIZE_SCALE);
    //récupération et modification des composants
    mMoteurGraphique.memorizeSizeTile(niv.getLongueurTile(), niv.getLargeurTile());

}

bool Moteur::loadPlayersAndBot(unsigned int uiNumPlayer, unsigned int uiNumBot)
{
    unsigned int memBombermanSprite;
    if(MAX_PLAYER < uiNumPlayer + uiNumBot)return false;
    memBombermanSprite = mMoteurGraphique.loadSprite(TEXTURE_BOMBERMAN, sf::IntRect(71, 45, 16, 25));
    mMoteurGraphique.loadSprite(TEXTURE_EXPLOSION, sf::IntRect(51, 0, 15, 15));
    unsigned int largeurTile = mPtrJeu.getNiveau().getLargeurTile();
    unsigned int longueurTile = mPtrJeu.getNiveau().getLongueurTile();
    for(unsigned int i = 0 ; i < uiNumPlayer ; ++i)
	{
        std::vector<bool> bitsetComp;
        bitsetComp.resize(getGestionnaireECS().getECSComponentManager()->getNumberComponent());
        bitsetComp[ecs::DISPLAY_COMPONENT]          = true;
        bitsetComp[ecs::POSITION_COMPONENT]         = true;
        bitsetComp[MOVEABLE_BOMBER_COMPONENT]       = true;
        bitsetComp[ecs::COLL_RECTBOX_COMPONENT]     = true;
        bitsetComp[INPUT_BOMBER_COMPONENT]          = true;
        bitsetComp[FLAG_BOMBER_COMPONENT]           = true;
        bitsetComp[PLAYER_CONFIG_BOMBER_COMPONENT]  = true;
        bitsetComp[TIMER_BOMBER_COMPONENT]          = true;

        unsigned int memEntity = mGestECS.addEntity(bitsetComp);
		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<InputBombermanComponent>());

		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<MoveableBombermanComponent>());
		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<FlagBombermanComponent>());
        mGestECS.getECSComponentManager()->
                instanciateExternComponent(memEntity, std::make_unique<PlayerConfigBombermanComponent>());
        mGestECS.getECSComponentManager()->
                instanciateExternComponent(memEntity, std::make_unique<TimerBombermanComponent>());

        FlagBombermanComponent *fc = mGestECS.getECSComponentManager()->
                searchComponentByType <FlagBombermanComponent> (memEntity, FLAG_BOMBER_COMPONENT);
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

        ecs::PositionComponent * pc = mGestECS.getECSComponentManager() ->
                searchComponentByType< ecs::PositionComponent >(memEntity, ecs::POSITION_COMPONENT);
        assert( pc && "pc == null\n");

        PlayerConfigBombermanComponent * playerConfig = mGestECS.getECSComponentManager() ->
                searchComponentByType<PlayerConfigBombermanComponent>(memEntity, PLAYER_CONFIG_BOMBER_COMPONENT);
        assert(playerConfig && "pc == null\n");

        const vectPairUi_t &memInitPosition = Niveau::static_getVectInitPositionBomberman();

        playerConfig->mInitX = memInitPosition[i].first;
        playerConfig->mInitY = memInitPosition[i].second;

        switch(i)
        {
        case 0:
            mMoteurGraphique.static_positionnerCaseTileMap(*pc, 1, 1);
            break;
        case 1:
            mMoteurGraphique.static_positionnerCaseTileMap(*pc, 9, 1);
            break;
        }
	}
	return true;
	//uiNumBot a implémenter ultérieurement
}

void Moteur::loadLevelWall(const Niveau &niv)
{
	std::vector< bool > bitsetComp(getGestionnaireECS().getECSComponentManager()->getNumberComponent());
    TilemapBombermanComponent *tmc = mGestECS.getECSComponentManager() ->
            searchComponentByType<TilemapBombermanComponent>(niv.getNumEntityLevel(), TILEMAP_BOMBER_COMPONENT);

    assert(tmc && "level TilemapBombermanComponent is null");

    const std::vector<unsigned char> &memTabNiv = tmc->mTabTilemap.getTab();
    unsigned int longueurNiveau = niv.getLongueurNiveau();
    unsigned int largeurTile = mPtrJeu.getNiveau().getLargeurTile();
    unsigned int longueurTile = mPtrJeu.getNiveau().getLongueurTile();
    unsigned int cmptX = 0, cmptY = 0;
    std::vector<unsigned char>::const_iterator it = memTabNiv.begin();
    for(; it != memTabNiv.end(); ++it)
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
        bitsetComp[ FLAG_BOMBER_COMPONENT ] = true;

		unsigned int memEntity = mGestECS.addEntity( bitsetComp );
        //memorize entity number for destructible wall

        if(*it == TILE_DESTRUCTIBLE_WALL)
        {
            if(! Niveau::static_setNumWallEntityOnPosition(cmptX, cmptY, memEntity))
            {
                assert(false && "Value destruct wall position is out of bound.");
            }
        }
		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<FlagBombermanComponent>());

		FlagBombermanComponent *fc = mGestECS.getECSComponentManager()->
                searchComponentByType < FlagBombermanComponent > ( memEntity, FLAG_BOMBER_COMPONENT );
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

bool Moteur::stopGame() const
{
    return mLetMeOut;
}

void Moteur::positionnerComponent(ecs::PositionComponent &posComp, unsigned int posX, unsigned int posY)
{
    posComp.vect2DPosComp.mfX = POSITION_LEVEL_X + posX * mPtrJeu.getNiveau().getLongueurTile();
    posComp.vect2DPosComp.mfY = POSITION_LEVEL_Y + posY * mPtrJeu.getNiveau().getLargeurTile();
//	std::cout << "XX " << posX << " posComp.X " <<posComp.vect2DPosComp.mfX
//			  << " YY " << posY << " posComp.Y " <<posComp.vect2DPosComp.mfY<< std::endl;
}
