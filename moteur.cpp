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
    const std::vector<uint32_t> &vectNumEntitySystem = mGestECS.getECSSystemManager() ->
			searchSystemByType< InputBombermanSystem > ( INPUT_BOMBER_SYSTEM )->getVectNumEntity();

    for( uint32_t i = 0 ; i < vectNumEntitySystem.size() ; ++i )
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

void Moteur::loadLevelTileMap(Niveau &niv, uint32_t numNiv)
{
    mMoteurGraphique.loadLevelTileMap(niv, numNiv);
}

GestionnaireECS &Moteur::getGestionnaireECS()
{
	return mGestECS;
}

uint32_t Moteur::initLevel()
{
    uint32_t memEntity;
    //création de l'entité avec les composants nécessaires
    std::vector<bool> bitsetComp;
    bitsetComp.resize(getGestionnaireECS().getECSComponentManager()->getNumberComponent());
    bitsetComp[TILEMAP_BOMBER_COMPONENT] = true;
    bitsetComp[ecs::POSITION_COMPONENT] = true;
    memEntity = mGestECS.addEntity(bitsetComp);
    mGestECS.getECSComponentManager()->instanciateExternComponent(memEntity, std::make_unique<TilemapBombermanComponent>());

    ecs::PositionComponent *pc = mGestECS.getECSComponentManager() ->
            searchComponentByType< ecs::PositionComponent >(memEntity, ecs::POSITION_COMPONENT);
    assert(pc && "Moteur::initLevel PositionComponent == null\n");
    pc->vect2DPosComp.mfX = POSITION_LEVEL_X;
    pc->vect2DPosComp.mfY = POSITION_LEVEL_Y;
    return memEntity;
}


bool Moteur::loadPlayersAndBot(uint32_t uiNumPlayer, uint32_t uiNumBot)
{
    if(MAX_PLAYER < uiNumPlayer + uiNumBot)return false;
    mMoteurGraphique.loadBaseSprites();
    for(uint32_t i = 0 ; i < uiNumPlayer ; ++i)
	{
        createBomberman(i);
	}
	return true;
    //uiNumBot a implémenter ultérieurement
}

void Moteur::createBomberman(uint32_t numPlayer)
{
    assert(numPlayer < MAX_PLAYER && "Player number out of bound.");
    std::vector<bool> bitsetComp;
    fillBombermanEntityBitset(bitsetComp, numPlayer);
    uint32_t memEntity = instanciateBombermanComponents(bitsetComp);
    configBombermanComponents(memEntity, numPlayer, bitsetComp);
}

void Moteur::fillBombermanEntityBitset(std::vector<bool> &bombermanBitset,
                                       uint32_t uiNumPlayer)
{
    bombermanBitset.resize(getGestionnaireECS().getECSComponentManager()->getNumberComponent());
    bombermanBitset[ecs::DISPLAY_COMPONENT]          = true;
    bombermanBitset[ecs::POSITION_COMPONENT]         = true;
    bombermanBitset[MOVEABLE_BOMBER_COMPONENT]       = true;
    bombermanBitset[ecs::COLL_RECTBOX_COMPONENT]     = true;
    bombermanBitset[FLAG_BOMBER_COMPONENT]           = true;
    bombermanBitset[PLAYER_CONFIG_BOMBER_COMPONENT]  = true;
    bombermanBitset[TIMER_BOMBER_COMPONENT]          = true;
    if((uiNumPlayer == 0 && Jeu::getGameMode() != GameMode::CLIENT) ||
            (uiNumPlayer == 1 && Jeu::getGameMode() != GameMode::SERVER))
    {
        bombermanBitset[INPUT_BOMBER_COMPONENT]          = true;
    }
    if(Jeu::getGameMode() != GameMode::SOLO)
    {
        bombermanBitset[NETWORK_BOMBER_COMPONENT]        = true;
    }
}

uint32_t Moteur::instanciateBombermanComponents(std::vector<bool> &bombermanBitset)
{
    uint32_t memEntity = mGestECS.addEntity(bombermanBitset);
    if(bombermanBitset[INPUT_BOMBER_COMPONENT])
    {
        mGestECS.getECSComponentManager()->
                instanciateExternComponent(memEntity, std::make_unique<InputBombermanComponent>());
    }
    if(bombermanBitset[NETWORK_BOMBER_COMPONENT])
    {
        mGestECS.getECSComponentManager()->
                instanciateExternComponent(memEntity, std::make_unique<NetworkBombermanComponent>());
    }
    mGestECS.getECSComponentManager()->
            instanciateExternComponent(memEntity, std::make_unique<MoveableBombermanComponent>());
    mGestECS.getECSComponentManager()->
            instanciateExternComponent(memEntity, std::make_unique<FlagBombermanComponent>());
    mGestECS.getECSComponentManager()->
            instanciateExternComponent(memEntity, std::make_unique<PlayerConfigBombermanComponent>());
    mGestECS.getECSComponentManager()->
            instanciateExternComponent(memEntity, std::make_unique<TimerBombermanComponent>());

    return memEntity;
}

void Moteur::configBombermanComponents(uint32_t numEntity, uint32_t numPlayer,
                                       const std::vector<bool> &bombermanBitset)
{
    uint32_t memBombermanSprite = SPRITE_BOMBERMAN_DOWN_STATIC;
    uint32_t largeurTile = mPtrJeu.getNiveau().getLargeurTile();
    uint32_t longueurTile = mPtrJeu.getNiveau().getLongueurTile();
    if(bombermanBitset[NETWORK_BOMBER_COMPONENT])
    {
        NetworkBombermanComponent *nb = mGestECS.getECSComponentManager()->
                searchComponentByType<NetworkBombermanComponent> (numEntity, NETWORK_BOMBER_COMPONENT);
        nb->mEntityType = TypeEntityFlag::FLAG_BOMBERMAN;
        nb->mNetworkId = NetworkBombermanComponent::attributeNum();
        //PLAYER 0 == SERVER PLAYER
        if(numPlayer == 0)
        {
            nb->mServerEntity = true;
        }
        else
        {
            nb->mServerEntity = false;
        }

    }
    FlagBombermanComponent *fc = mGestECS.getECSComponentManager()->
            searchComponentByType <FlagBombermanComponent> (numEntity, FLAG_BOMBER_COMPONENT);
    assert(fc && "dc == null\n");
    fc->muiNumFlag = FLAG_BOMBERMAN;

    ecs::CollRectBoxComponent * cc = mGestECS.getECSComponentManager() ->
            searchComponentByType< ecs::CollRectBoxComponent >(numEntity, ecs::COLL_RECTBOX_COMPONENT);
    assert(cc && "dc == null\n");

    //offset
    cc->mVect2dVectOrigins.mfX = 5;
    cc->mRectBox.mSetLenghtRectBox(longueurTile - 10);
    cc->mVect2dVectOrigins.mfY = 56;
    cc->mRectBox.mSetHeightRectBox(largeurTile - 10);
    ecs::DisplayComponent * dc = mGestECS.getECSComponentManager() ->
            searchComponentByType< ecs::DisplayComponent >( numEntity, ecs::DISPLAY_COMPONENT );
    assert(dc && "dc == null\n");
    dc->muiNumSprite = memBombermanSprite;

    ecs::PositionComponent * pc = mGestECS.getECSComponentManager() ->
            searchComponentByType< ecs::PositionComponent >(numEntity, ecs::POSITION_COMPONENT);
    assert(pc && "pc == null\n");

    PlayerConfigBombermanComponent * playerConfig = mGestECS.getECSComponentManager() ->
            searchComponentByType<PlayerConfigBombermanComponent>(numEntity, PLAYER_CONFIG_BOMBER_COMPONENT);
    assert(playerConfig && "pc == null\n");
    const vectPairUi_t &memInitPosition = Niveau::static_getVectInitPositionBomberman();

    playerConfig->mInitX = memInitPosition[numPlayer].first;
    playerConfig->mInitY = memInitPosition[numPlayer].second;

    InputBombermanComponent * inputComp = nullptr;
    if(bombermanBitset[INPUT_BOMBER_COMPONENT])
    {
        inputComp = mGestECS.getECSComponentManager() ->
                searchComponentByType<InputBombermanComponent>(numEntity, INPUT_BOMBER_COMPONENT);
        assert(inputComp && "inputComp == null\n");
    }
    mMoteurGraphique.static_positionnerCaseTileMap(*pc, Niveau::static_getVectInitPositionBomberman()[numPlayer].first,
                                                   Niveau::static_getVectInitPositionBomberman()[numPlayer].second);

    if(bombermanBitset[INPUT_BOMBER_COMPONENT])
    {
        if(numPlayer == 0 && Jeu::getGameMode() != GameMode::CLIENT)
        {
            inputComp->mNumInput = INPUT_PLAYER_A;
        }
    }
    if(bombermanBitset[INPUT_BOMBER_COMPONENT])
    {
        // == 1 pour les tests
        if(numPlayer == 1 && Jeu::getGameMode() != GameMode::SERVER)
        {
            inputComp->mNumInput = INPUT_PLAYER_B;
        }
    }
}

void Moteur::loadLevelWall(const Niveau &niv)
{
	std::vector< bool > bitsetComp(getGestionnaireECS().getECSComponentManager()->getNumberComponent());
    TilemapBombermanComponent *tmc = mGestECS.getECSComponentManager() ->
            searchComponentByType<TilemapBombermanComponent>(niv.getNumEntityLevel(), TILEMAP_BOMBER_COMPONENT);

    assert(tmc && "level TilemapBombermanComponent is null");

    const std::vector<unsigned char> &memTabNiv = tmc->mTabTilemap.getTab();
    uint32_t longueurNiveau = niv.getLongueurNiveau();
    uint32_t largeurTile = mPtrJeu.getNiveau().getLargeurTile();
    uint32_t longueurTile = mPtrJeu.getNiveau().getLongueurTile();
    uint32_t cmptX = 0, cmptY = 0;
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

        uint32_t memEntity = mGestECS.addEntity(bitsetComp);
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
    sss->sendData("127.0.0.1", SERVER_PORT);
    synchLevelFromServer(*sss, niv);
    loadPlayersAndBot(2, 0);
    synchPlayersFromServer(*sss);
    sss->launchReceptThread(false, false);
}

void Moteur::synchLevelFromServer(SocketSystem &socketSystem, Niveau &niv)
{
    socketSystem.receiveData(false, true);
    NetworkLevelData levelData;
    socketSystem.clientSyncNetworkLevel(levelData);
    mMoteurGraphique.loadLevelTileMapFromServer(niv, levelData);
}

void Moteur::synchPlayersFromServer(SocketSystem &socketSystem)
{
    socketSystem.receiveData(false, true);
    socketSystem.clientSyncNetworkID();
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

void Moteur::positionnerComponent(ecs::PositionComponent &posComp, uint32_t posX, uint32_t posY)
{
    posComp.vect2DPosComp.mfX = POSITION_LEVEL_X + posX * mPtrJeu.getNiveau().getLongueurTile();
    posComp.vect2DPosComp.mfY = POSITION_LEVEL_Y + posY * mPtrJeu.getNiveau().getLargeurTile();
//	std::cout << "XX " << posX << " posComp.X " <<posComp.vect2DPosComp.mfX
    //			  << " YY " << posY << " posComp.Y " <<posComp.vect2DPosComp.mfY<< std::endl;
}
