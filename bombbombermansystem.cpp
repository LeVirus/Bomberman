#include "bombbombermansystem.hpp"
#include "timerbombermancomponent.hpp"
#include "flagcomponent.hpp"
#include "positioncomponent.hpp"
#include "displaycomponent.hpp"
#include "engine.hpp"
#include <cassert>

BombBombermanSystem::BombBombermanSystem() : mTimeExplode(3000)
{
//    if(! bAddComponentToSystem(ecs::POSITION_COMPONENT))
//    {
//        std::cout << "Erreur BombBombermanSystem ajout POSITION_COMPONENT.\n";
//    }
//    if(! bAddComponentToSystem(ecs::DISPLAY_COMPONENT))
//    {
//        std::cout << "Erreur BombBombermanSystem ajout DISPLAY_COMPONENT.\n";
//    }
//    if(! bAddComponentToSystem(BOMBER_FLAG_COMPONENT))
//    {
//        std::cout << "Erreur BombBombermanSystem ajout BOMBER_FLAG_COMPONENT.\n";
//    }
    if(! bAddComponentToSystem(BOMBER_TIMER_COMPONENT))
    {
        std::cout << "Erreur BombBombermanSystem ajout BOMBER_TIMER_COMPONENT.\n";
    }
}

void BombBombermanSystem::execSystem()
{
    System::execSystem();
    std::vector< unsigned int >::iterator it = mVectNumEntity.begin();
    std::cout << "BOmb\n";
    TimerBombermanComponent* timerComp;
    for(; it != mVectNumEntity.end() ; ++it)
    {
        std::cout << *it <<" BOmb\n";

        timerComp = stairwayToComponentManager() .
                    searchComponentByType <TimerBombermanComponent> (*it, BOMBER_TIMER_COMPONENT);
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
                makeBombExplode(*it);
            }
        }
    }
}

void BombBombermanSystem::makeBombExplode(unsigned int numEntity)
{
    mptrSystemManager->getptrEngine()->bRmEntity(numEntity);
    std::cout << numEntity << "EXPLOOOODE!!!!\n";
}

void BombBombermanSystem::displaySystem() const
{}

void BombBombermanSystem::lauchBomb(unsigned int numEntity, const ecs::PositionComponent &posA)
{
    unsigned int numCreatedEntity = mptrSystemManager->getptrEngine()->AddEntity();
    ecs::Engine *ECSEngine = mptrSystemManager->getptrEngine();
    ECSEngine->bAddComponentToEntity( numCreatedEntity, ecs::DISPLAY_COMPONENT );
    ECSEngine->bAddComponentToEntity( numCreatedEntity, ecs::POSITION_COMPONENT );
    ECSEngine->bAddComponentToEntity( numCreatedEntity, BOMBER_FLAG_COMPONENT );
    ECSEngine->bAddComponentToEntity( numCreatedEntity, BOMBER_TIMER_COMPONENT );
    stairwayToComponentManager().updateComponentFromEntity();

    ecs::PositionComponent *posComponent = stairwayToComponentManager().searchComponentByType < ecs::PositionComponent >
            (numCreatedEntity, ecs::POSITION_COMPONENT);
    assert(posComponent && "BombBombermanSystem::lauchBomb posComponent is null\n");
    posComponent->vect2DPosComp.mfX = posA.vect2DPosComp.mfX;
    posComponent->vect2DPosComp.mfY = posA.vect2DPosComp.mfY;

    ecs::DisplayComponent *dispComponent = stairwayToComponentManager().searchComponentByType < ecs::DisplayComponent >
            (numCreatedEntity, ecs::DISPLAY_COMPONENT);
    assert(posComponent && "BombBombermanSystem::lauchBomb dispComponent is null\n");

    dispComponent->muiNumSprite = SPRITE_BOMB;

    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<FlagBombermanComponent>());
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<TimerBombermanComponent>());


}
