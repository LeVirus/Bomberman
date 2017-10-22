#include "bombbombermansystem.hpp"
#include "timerbombermancomponent.hpp"
#include "flagcomponent.hpp"
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

    stairwayToComponentManager().instanciateExternComponent(numEntity, std::make_unique<FlagBombermanComponent>());
    stairwayToComponentManager().instanciateExternComponent(numEntity, std::make_unique<TimerBombermanComponent>());


}
