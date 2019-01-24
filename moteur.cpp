#include "moteur.hpp"
#include "tilemap.hpp"
#include "NetworkDataUtils.hpp"
#include "networkserialstruct.hpp"
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
#include "networkcomponent.hpp"
#include "jeu.hpp"
#include "socketsystem.hpp"

#include "displaysystem.hpp"
#include "positioncomponent.hpp"
#include "timerbombermancomponent.hpp"

#include <SFML/Graphics.hpp>
#include <bitset>
#include <cassert>
#include <SFML/System/Clock.hpp>

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
    assert(ss && "ss null");
    const std::vector<unsigned int> &vectNumEntitySystem = mGestECS.getECSSystemManager() ->
			searchSystemByType< InputBombermanSystem > ( INPUT_BOMBER_SYSTEM )->getVectNumEntity();

	for( unsigned int i = 0 ; i < vectNumEntitySystem.size() ; ++i )
	{

		InputBombermanComponent *ic = mGestECS.getECSComponentManager() ->
				searchComponentByType< InputBombermanComponent >(
                    vectNumEntitySystem[i], INPUT_BOMBER_COMPONENT );
        if(! ic)continue;
        sf::Keyboard::Key up = sf::Keyboard::Z, down = sf::Keyboard::S, left = sf::Keyboard::Q,
                right = sf::Keyboard::D, launchBomb = sf::Keyboard::Space;
        switch(ic->mNumInput)
        {
            case INPUT_PLAYER_B:
            up = sf::Keyboard::Up;
            down = sf::Keyboard::Down;
            left = sf::Keyboard::Left;
            right = sf::Keyboard::Right;
            launchBomb = sf::Keyboard::RControl;
            break;
        }
        if(sf::Keyboard::isKeyPressed(up))ic->mBitsetInput[MOVE_UP] = true;
        if(sf::Keyboard::isKeyPressed(down))ic->mBitsetInput[MOVE_DOWN] = true;
        if(sf::Keyboard::isKeyPressed(left))ic->mBitsetInput[MOVE_LEFT] = true;
        if(sf::Keyboard::isKeyPressed(right))ic->mBitsetInput[MOVE_RIGHT] = true;
        if(sf::Keyboard::isKeyPressed(launchBomb))ic->mBitsetInput[LAUNCH_BOMB] = true;
	}
}

GestionnaireECS &Moteur::getGestionnaireECS()
{
	return mGestECS;
}

unsigned int Moteur::initLevel()
{
    unsigned int memEntity;
    //création de l'entité avec les composants nécessaires
    std::vector<bool> bitsetComp;
    bitsetComp.resize(getGestionnaireECS().getECSComponentManager()->getNumberComponent());
    bitsetComp[TILEMAP_BOMBER_COMPONENT] = true;
    bitsetComp[ecs::POSITION_COMPONENT] = true;
    memEntity = mGestECS.addEntity(bitsetComp);
    mGestECS.getECSComponentManager()->instanciateExternComponent(memEntity, std::make_unique<TilemapBombermanComponent>());

    ecs::PositionComponent *pc = mGestECS.getECSComponentManager() ->
            searchComponentByType< ecs::PositionComponent >(memEntity, ecs::POSITION_COMPONENT);
    assert(pc && "Moteur::loadTileMap PositionComponent == null\n");
    pc->vect2DPosComp.mfX = POSITION_LEVEL_X;
    pc->vect2DPosComp.mfY = POSITION_LEVEL_Y;
    return memEntity;
}

void Moteur::loadLevelTileMap(Niveau &niv, unsigned int numNiv)
{
    unsigned int memEntity = initLevel();
    TilemapBombermanComponent *tmc = mGestECS.getECSComponentManager() ->
            searchComponentByType<TilemapBombermanComponent>(memEntity, TILEMAP_BOMBER_COMPONENT);
    assert(tmc && "Moteur::loadTileMap TilemapBombermanComponent == null\n");
    niv.loadLevel(numNiv, memEntity, *tmc);
    niv.adaptToScale(SIZE_SCALE, SIZE_SCALE);
    //récupération et modification des composants
    mMoteurGraphique.memorizeSizeTile(niv.getLongueurTile(), niv.getLargeurTile());
}

void Moteur::loadLevelTileMapFromServer(Niveau &niv, const NetworkLevelData &dataLevel)
{
    unsigned int memEntity = initLevel();
    TilemapBombermanComponent *tmc = mGestECS.getECSComponentManager() ->
            searchComponentByType<TilemapBombermanComponent>(memEntity, TILEMAP_BOMBER_COMPONENT);
    assert(tmc && "Moteur::loadTileMap TilemapBombermanComponent == null");
    bool res = niv.loadLevelFromServer(memEntity, *tmc, dataLevel);
    assert(res && "loadLevelFromServer fail");
    niv.adaptToScale(SIZE_SCALE, SIZE_SCALE);
    //récupération et modification des composants
    mMoteurGraphique.memorizeSizeTile(niv.getLongueurTile(), niv.getLargeurTile());
}

bool Moteur::loadPlayersAndBot(unsigned int uiNumPlayer, unsigned int uiNumBot)
{
    unsigned int memBombermanSprite = SPRITE_BOMBERMAN_DOWN_STATIC;
    if(MAX_PLAYER < uiNumPlayer + uiNumBot)return false;
    loadBombermanSprite();
    LoadBomBSprite();
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
        bitsetComp[FLAG_BOMBER_COMPONENT]           = true;
        bitsetComp[PLAYER_CONFIG_BOMBER_COMPONENT]  = true;
        bitsetComp[TIMER_BOMBER_COMPONENT]          = true;

        if((i == 0 && Jeu::getGameMode() != GameMode::CLIENT) ||
                (i == 1 && Jeu::getGameMode() != GameMode::SERVER))
        {
            bitsetComp[INPUT_BOMBER_COMPONENT]          = true;
        }
        if(Jeu::getGameMode() != GameMode::SOLO)
        {
            bitsetComp[NETWORK_BOMBER_COMPONENT]        = true;
        }

        unsigned int memEntity = mGestECS.addEntity(bitsetComp);
        if(bitsetComp[INPUT_BOMBER_COMPONENT])
        {
            mGestECS.getECSComponentManager()->
                    instanciateExternComponent(memEntity, std::make_unique<InputBombermanComponent>());
        }

		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<MoveableBombermanComponent>());
		mGestECS.getECSComponentManager()->
				instanciateExternComponent(memEntity, std::make_unique<FlagBombermanComponent>());
        mGestECS.getECSComponentManager()->
                instanciateExternComponent(memEntity, std::make_unique<PlayerConfigBombermanComponent>());
        mGestECS.getECSComponentManager()->
                instanciateExternComponent(memEntity, std::make_unique<TimerBombermanComponent>());

        if(bitsetComp[NETWORK_BOMBER_COMPONENT])
        {
            mGestECS.getECSComponentManager()->
                    instanciateExternComponent(memEntity, std::make_unique<NetworkBombermanComponent>());

            NetworkBombermanComponent *nb = mGestECS.getECSComponentManager()->
                    searchComponentByType<NetworkBombermanComponent> (memEntity, NETWORK_BOMBER_COMPONENT);
            nb->mEntityType = TypeEntityFlag::FLAG_BOMBERMAN;
            if(Jeu::getGameMode() == GameMode::SERVER)
            {
                nb->mNetworkId = NetworkBombermanComponent::attributeNum();
            }
        }

        FlagBombermanComponent *fc = mGestECS.getECSComponentManager()->
                searchComponentByType <FlagBombermanComponent> (memEntity, FLAG_BOMBER_COMPONENT);
        assert(fc && "dc == null\n");
        fc->muiNumFlag = FLAG_BOMBERMAN;

		ecs::CollRectBoxComponent * cc = mGestECS.getECSComponentManager() ->
                searchComponentByType< ecs::CollRectBoxComponent >(memEntity, ecs::COLL_RECTBOX_COMPONENT);
        assert(cc && "dc == null\n");

        //offset
        cc->mVect2dVectOrigins.mfX = 5;
        cc->mRectBox.mSetLenghtRectBox(longueurTile - 10);
        cc->mVect2dVectOrigins.mfY = 56;
        cc->mRectBox.mSetHeightRectBox(largeurTile - 10);
        ecs::DisplayComponent * dc = mGestECS.getECSComponentManager() ->
                searchComponentByType< ecs::DisplayComponent >( memEntity, ecs::DISPLAY_COMPONENT );
        assert(dc && "dc == null\n");
        dc->muiNumSprite = memBombermanSprite;

        ecs::PositionComponent * pc = mGestECS.getECSComponentManager() ->
                searchComponentByType< ecs::PositionComponent >(memEntity, ecs::POSITION_COMPONENT);
        assert(pc && "pc == null\n");

        PlayerConfigBombermanComponent * playerConfig = mGestECS.getECSComponentManager() ->
                searchComponentByType<PlayerConfigBombermanComponent>(memEntity, PLAYER_CONFIG_BOMBER_COMPONENT);
        assert(playerConfig && "pc == null\n");
        const vectPairUi_t &memInitPosition = Niveau::static_getVectInitPositionBomberman();

        playerConfig->mInitX = memInitPosition[i].first;
        playerConfig->mInitY = memInitPosition[i].second;

        InputBombermanComponent * inputComp = nullptr;
        if(bitsetComp[INPUT_BOMBER_COMPONENT])
        {
            inputComp = mGestECS.getECSComponentManager() ->
                    searchComponentByType<InputBombermanComponent>(memEntity, INPUT_BOMBER_COMPONENT);
            assert(inputComp && "inputComp == null\n");
        }
        if(i == 0)
        {
            mMoteurGraphique.static_positionnerCaseTileMap(*pc, 1, 1);
            if(bitsetComp[INPUT_BOMBER_COMPONENT])
            {
                inputComp->mNumInput = INPUT_PLAYER_A;
            }
        }
        else if(i == 1)
        {
            mMoteurGraphique.static_positionnerCaseTileMap(*pc, 9, 1);
            if(bitsetComp[INPUT_BOMBER_COMPONENT])
            {
                if(Jeu::getGameMode() == GameMode::CLIENT)
                {
                    inputComp->mNumInput = INPUT_PLAYER_A;
                }
                else
                {
                    inputComp->mNumInput = INPUT_PLAYER_B;
                }
            }
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
        if(*it > TILE_DESTRUCTIBLE_WALL)
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

        unsigned int memEntity = mGestECS.addEntity(bitsetComp);
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
                searchComponentByType <FlagBombermanComponent> ( memEntity, FLAG_BOMBER_COMPONENT );
        fc->muiNumFlag = FLAG_DESTRUCTIBLE_WALL;

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

void Moteur::synchronizeEntitiesNetworkIdToClients()
{
    SocketSystem * sss = getSocketSystem();
    sss->execSystem();
}

void Moteur::waitServerSync(Niveau &niv)
{
    SocketSystem * sss = getSocketSystem();
    assert(sss && "SocketSystem == nullptr");
    sss->receiveData(true);
    NetworkLevelData levelData;
    sss->clientSyncNetworkLevel(levelData);
    loadLevelTileMapFromServer(niv, levelData);
    loadPlayersAndBot(2, 0);
    sss->receiveData(true);
    sss->clientSyncNetworkID();
}

void Moteur::synchronizeLevelToClients(const Niveau &level)
{
    SocketSystem *sss = getSocketSystem();
    assert(sss && "SocketSystem == nullptr");
    sss->synchronizeLevelToClients(level);
}

SocketSystem *Moteur::getSocketSystem()
{
    SocketSystem *sss = mGestECS.getECSSystemManager()->
            searchSystemByType<SocketSystem>(SOCKET_SYSTEM);
    assert(sss && "SocketSystem == nullptr");
    return sss;
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

void Moteur::loadBombermanSprite()
{
    unsigned int cursorX = 3, cursorY = 19;
    for(unsigned int i = 0 ; i < 18; ++i)
    {
        if(i == 3 || i == 12)
        {
            cursorY += 26;
            cursorX = 3;
        }
        mMoteurGraphique.loadSprite(TEXTURE_BOMBERMAN, sf::IntRect(cursorX, cursorY, 16, 25));
        cursorX += 17;
    }
}

void Moteur::LoadBomBSprite()
{
    unsigned int cursorX = 0, cursorY = 0;
    for(unsigned int i = 0 ; i < 3; ++i)
    {
        mMoteurGraphique.loadSprite(TEXTURE_EXPLOSION, sf::IntRect(cursorX, cursorY, 16, 16));
        cursorX += 17;
    }
}
