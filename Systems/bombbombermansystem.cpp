#include "bombbombermansystem.hpp"
#include "timerbombermancomponent.hpp"
#include "flagcomponent.hpp"
#include "displaycomponent.hpp"
#include "positioncomponent.hpp"
#include "bombconfigbombermancomponent.hpp"
#include "collrectboxcomponent.hpp"
#include "playerconfigbombermancomponent.hpp"
#include "displaycomponent.hpp"
#include "networkcomponent.hpp"
#include "moteurgraphique.hpp"
#include "engine.hpp"
#include "jeu.hpp"
#include <cassert>
#include "explosionbombermansystem.hpp"



BombBombermanSystem::BombBombermanSystem()
{
    if(! bAddComponentToSystem(FLAG_BOMBER_COMPONENT))
    {
        std::cout << "Erreur BombBombermanSystem ajout BOMBER_FLAG_COMPONENT.\n";
    }
    if(! bAddComponentToSystem(TIMER_BOMBER_COMPONENT))
    {
        std::cout << "Erreur BombBombermanSystem ajout BOMBER_TIMER_COMPONENT.\n";
    }
    if(! bAddComponentToSystem(BOMB_CONFIG_BOMBER_COMPONENT))
    {
        std::cout << "Erreur BombBombermanSystem ajout BOMB_CONFIG_BOMBER_COMPONENT.\n";
    }
}

void BombBombermanSystem::execSystem()
{
    System::execSystem();
    std::vector< unsigned int >::iterator it = mVectNumEntity.begin();
    for(; it != mVectNumEntity.end() ; ++it)
    {

        FlagBombermanComponent *flagComponent = stairwayToComponentManager().searchComponentByType <FlagBombermanComponent>
                (*it, FLAG_BOMBER_COMPONENT);

        assert(flagComponent && "BombBombermanSystem::execSystem flagComponent is null\n");
        if(flagComponent->muiNumFlag != FLAG_BOMB)
        {
            continue;
        }

        TimerBombermanComponent* timerComp = stairwayToComponentManager() .
                    searchComponentByType <TimerBombermanComponent> (*it, TIMER_BOMBER_COMPONENT);
        assert(timerComp && "BombBombermanSystem::execSystem :: timerComp == NULL\n");

        BombConfigBombermanComponent* bombConfComp = stairwayToComponentManager() .
                    searchComponentByType <BombConfigBombermanComponent> (*it, BOMB_CONFIG_BOMBER_COMPONENT);
        assert(bombConfComp && "bombConfComp == NULL\n");
        if(timerComp->mBombClockB.getElapsedTime().asMilliseconds() > 400)
        {
            changeSpriteBomb(*it, bombConfComp->mPreviousSizeLittle);
            timerComp->mBombClockB.restart();
        }

        if(! timerComp->mLaunched)
        {
            timerComp->mBombClock.restart();
            timerComp->mLaunched = true;
        }
        else
        {
            unsigned int timeElapsed = timerComp->mBombClock.getElapsedTime().asMilliseconds();
            if(timeElapsed >= bombConfComp->mTimeBeforeExplosion)
            {

                if(! m_ptrExplosionSystem)
                {
                    m_ptrExplosionSystem = mptrSystemManager->searchSystemByType<ExplosionBombermanSystem>(EXPLOSION_BOMBER_SYSTEM);
                    assert(m_ptrExplosionSystem && "BombBombermanSystem::BombBombermanSystem explosionSystem is null\n");
                }
                m_ptrExplosionSystem->makeBombExplode(*it);
                mptrSystemManager->getptrEngine()->bRmEntity(*it);
            }
        }
    }
}

void BombBombermanSystem::displaySystem() const
{}

unsigned int BombBombermanSystem::createBombEntity(bool network)
{
    //create entity
    unsigned int numCreatedEntity = mptrSystemManager->getptrEngine()->AddEntity();
    ecs::Engine *ECSEngine = mptrSystemManager->getptrEngine();
    ECSEngine->bAddComponentToEntity(numCreatedEntity, ecs::DISPLAY_COMPONENT);
    ECSEngine->bAddComponentToEntity(numCreatedEntity, ecs::POSITION_COMPONENT);
    ECSEngine->bAddComponentToEntity(numCreatedEntity, ecs::COLL_RECTBOX_COMPONENT);
    ECSEngine->bAddComponentToEntity(numCreatedEntity, FLAG_BOMBER_COMPONENT);
    ECSEngine->bAddComponentToEntity(numCreatedEntity, TIMER_BOMBER_COMPONENT);
    ECSEngine->bAddComponentToEntity(numCreatedEntity, BOMB_CONFIG_BOMBER_COMPONENT);
    if(network)
    {
        ECSEngine->bAddComponentToEntity(numCreatedEntity, NETWORK_BOMBER_COMPONENT);
    }

    stairwayToComponentManager().updateComponentFromEntity();
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<FlagBombermanComponent>());
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<TimerBombermanComponent>());
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<BombConfigBombermanComponent>());
    if(network)
    {
        stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<NetworkBombermanComponent>());
    }
    return numCreatedEntity;
}

void BombBombermanSystem::changeSpriteBomb(unsigned int numEntity, bool &previousSizeLittle)
{
    ecs::DisplayComponent *displayComp = stairwayToComponentManager() .
            searchComponentByType <ecs::DisplayComponent> (numEntity, ecs::DISPLAY_COMPONENT);
    assert(displayComp && "bombConfComp == NULL\n");
    if(displayComp->muiNumSprite == SPRITE_BOMB_LITTLE || displayComp->muiNumSprite == SPRITE_BOMB_BIG)
    {
        displayComp->muiNumSprite = SPRITE_BOMB_MEDIUM;
    }
    else if(previousSizeLittle)
    {
        previousSizeLittle = false;
        displayComp->muiNumSprite = SPRITE_BOMB_BIG;
    }
    else
    {
        previousSizeLittle = true;
        displayComp->muiNumSprite = SPRITE_BOMB_LITTLE;
    }
}

void BombBombermanSystem::lauchBomb(unsigned int numPlayerEntity,
                                    const ecs::PositionComponent &posA, bool network)
{
//    bool network = Jeu::getGameMode() != GameMode::SOLO;
    unsigned int numCreatedEntity = createBombEntity(network);

    //position entity
    ecs::PositionComponent *posComponent = stairwayToComponentManager().searchComponentByType < ecs::PositionComponent >
            (numCreatedEntity, ecs::POSITION_COMPONENT);
    assert(posComponent && "BombBombermanSystem::lauchBomb posComponent is null\n");
    posComponent->vect2DPosComp.mfX = posA.vect2DPosComp.mfX;
    posComponent->vect2DPosComp.mfY = posA.vect2DPosComp.mfY;

    ecs::CollRectBoxComponent *colComponent = stairwayToComponentManager().searchComponentByType<ecs::CollRectBoxComponent>
            (numCreatedEntity, ecs::COLL_RECTBOX_COMPONENT);
    assert(colComponent && "BombBombermanSystem::lauchBomb colComponent is null\n");
    colComponent->mRectBox.mSetHeightRectBox(Niveau::getLargeurTile());
    colComponent->mRectBox.mSetLenghtRectBox(Niveau::getLongueurTile());
    colComponent->mVect2dVectOrigins.mfX = 0;
    colComponent->mVect2dVectOrigins.mfY = 0;
    colComponent->mRectBox.mSetOriginsRectBox(posComponent->vect2DPosComp);

    MoteurGraphique::static_positionComponentCenterCurrentCase(*posComponent);

    PlayerConfigBombermanComponent *playerConfComponent = stairwayToComponentManager().searchComponentByType<PlayerConfigBombermanComponent>
            (numPlayerEntity, PLAYER_CONFIG_BOMBER_COMPONENT);
    assert(playerConfComponent && "BombBombermanSystem::lauchBomb playerConfComponent is null\n");
    BombConfigBombermanComponent *bombConfComponent = stairwayToComponentManager().searchComponentByType<BombConfigBombermanComponent>
            (numCreatedEntity, BOMB_CONFIG_BOMBER_COMPONENT);
    assert(bombConfComponent && "BombBombermanSystem::lauchBomb bombConfComponent is null\n");

    bombConfComponent->mRadiusExplosion = playerConfComponent->mRadiusExplosion;

    ecs::DisplayComponent *dispComponent = stairwayToComponentManager().searchComponentByType < ecs::DisplayComponent >
            (numCreatedEntity, ecs::DISPLAY_COMPONENT);
    assert(dispComponent && "BombBombermanSystem::lauchBomb dispComponent is null\n");

    dispComponent->muiNumSprite = SPRITE_BOMB_LITTLE;

    FlagBombermanComponent *flagComponent = stairwayToComponentManager().searchComponentByType <FlagBombermanComponent>
            (numCreatedEntity, FLAG_BOMBER_COMPONENT);
    assert(flagComponent && "BombBombermanSystem::lauchBomb flagComponent is null\n");
    flagComponent->muiNumFlag = FLAG_BOMB;

    if(network)
    {
        NetworkBombermanComponent *networkComp = stairwayToComponentManager().searchComponentByType <NetworkBombermanComponent>
                (numCreatedEntity, NETWORK_BOMBER_COMPONENT);
        assert(networkComp && "BombBombermanSystem::lauchBomb flagComponent is null\n");
        networkComp->mEntityType = TypeEntityFlag::FLAG_BOMB;
    }
}
