#include "bombbombermansystem.hpp"
#include "timerbombermancomponent.hpp"
#include "flagcomponent.hpp"
#include "positioncomponent.hpp"
#include "bombconfigbombermancomponent.hpp"
#include "displaycomponent.hpp"
#include "moteurgraphique.hpp"
#include "engine.hpp"
#include <cassert>
#include "explosionbombermansystem.hpp"



BombBombermanSystem::BombBombermanSystem() : mTimeExplode(500)
{
    if(! bAddComponentToSystem(FLAG_BOMBER_COMPONENT))
    {
        std::cout << "Erreur BombBombermanSystem ajout BOMBER_FLAG_COMPONENT.\n";
    }
    if(! bAddComponentToSystem(TIMER_BOMBER_COMPONENT))
    {
        std::cout << "Erreur BombBombermanSystem ajout BOMBER_TIMER_COMPONENT.\n";
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

        if(! timerComp->mLaunched)
        {
            timerComp->mBombClock.restart();
            timerComp->mLaunched = true;
        }
        else
        {
            unsigned int timeElapsed = timerComp->mBombClock.getElapsedTime().asMilliseconds();
            if(timeElapsed >= mTimeExplode)
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

unsigned int BombBombermanSystem::createBombEntity()
{
    //create entity
    unsigned int numCreatedEntity = mptrSystemManager->getptrEngine()->AddEntity();
    ecs::Engine *ECSEngine = mptrSystemManager->getptrEngine();
    ECSEngine->bAddComponentToEntity(numCreatedEntity, ecs::DISPLAY_COMPONENT);
    ECSEngine->bAddComponentToEntity(numCreatedEntity, ecs::POSITION_COMPONENT);
    ECSEngine->bAddComponentToEntity(numCreatedEntity, FLAG_BOMBER_COMPONENT);
    ECSEngine->bAddComponentToEntity(numCreatedEntity, TIMER_BOMBER_COMPONENT);
    ECSEngine->bAddComponentToEntity(numCreatedEntity, BOMB_CONFIG_BOMBER_COMPONENT);
    stairwayToComponentManager().updateComponentFromEntity();
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<FlagBombermanComponent>());
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<TimerBombermanComponent>());
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<BombConfigBombermanComponent>());
    return numCreatedEntity;
}

void BombBombermanSystem::lauchBomb(unsigned int numEntity, const ecs::PositionComponent &posA)
{
    unsigned int numCreatedEntity = createBombEntity();
    //position entity
    ecs::PositionComponent *posComponent = stairwayToComponentManager().searchComponentByType < ecs::PositionComponent >
            (numCreatedEntity, ecs::POSITION_COMPONENT);
    assert(posComponent && "BombBombermanSystem::lauchBomb posComponent is null\n");
    posComponent->vect2DPosComp.mfX = posA.vect2DPosComp.mfX;
    posComponent->vect2DPosComp.mfY = posA.vect2DPosComp.mfY;
    MoteurGraphique::static_positionComponentCenterCurrentCase(*posComponent);

    BombConfigBombermanComponent *bombConfComponent = stairwayToComponentManager().searchComponentByType<BombConfigBombermanComponent>
            (numCreatedEntity, BOMB_CONFIG_BOMBER_COMPONENT);
    assert(bombConfComponent && "BombBombermanSystem::lauchBomb bombConfComponent is null\n");
    bombConfComponent->mNumPlayerEntity = numEntity;

    ecs::DisplayComponent *dispComponent = stairwayToComponentManager().searchComponentByType < ecs::DisplayComponent >
            (numCreatedEntity, ecs::DISPLAY_COMPONENT);
    assert(dispComponent && "BombBombermanSystem::lauchBomb dispComponent is null\n");

    dispComponent->muiNumSprite = SPRITE_BOMB;

    FlagBombermanComponent *flagComponent = stairwayToComponentManager().searchComponentByType <FlagBombermanComponent>
            (numCreatedEntity, FLAG_BOMBER_COMPONENT);
    assert(flagComponent && "BombBombermanSystem::lauchBomb flagComponent is null\n");
    flagComponent->muiNumFlag = FLAG_BOMB;
}
