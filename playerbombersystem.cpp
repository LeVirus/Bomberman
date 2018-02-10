#include "playersystem.hpp"
#include "flagcomponent.hpp"
#include "timerbombermancomponent.hpp"
#include "playerconfigbombermancomponent.hpp"
#include "positioncomponent.hpp"
#include "constants.hpp"
#include "ECSconstantes.hpp"
#include "moteurgraphique.hpp"
#include "engine.hpp"
#include <cassert>

PlayerBomberSystem::PlayerBomberSystem()
{
//    if(! bAddComponentToSystem(BOMBER_FLAG_COMPONENT))
//    {
//        std::cout << "Erreur PlayerBomberSystem ajout BOMBER_FLAG_COMPONENT.\n";
//    }
    if(! bAddComponentToSystem(BOMBER_TIMER_COMPONENT))
    {
        std::cout << "Erreur PlayerBomberSystem ajout BOMBER_TIMER_COMPONENT.\n";
    }
    if(! bAddComponentToSystem(BOMBER_PLAYER_CONFIG_COMPONENT))
    {
        std::cout << "Erreur PlayerBomberSystem ajout BOMBER_PLAYER_CONFIG_COMPONENT.\n";
    }
}

void PlayerBomberSystem::execSystem()
{
    System::execSystem();
    std::vector<unsigned int>::iterator it = mVectNumEntity.begin();
    for(; it != mVectNumEntity.end() ; ++it)
    {
//        FlagBombermanComponent *flagComponent = stairwayToComponentManager().searchComponentByType <FlagBombermanComponent>
//                (*it, BOMBER_FLAG_COMPONENT);
//        assert(flagComponent && "PlayerBomberSystem::execSystem flagComponent is null\n");
//        if(flagComponent->muiNumFlag != FLAG_BOMBERMAN)
//        {
//            continue;
//        }

        PlayerConfigBombermanComponent *playerConfComponent = stairwayToComponentManager().searchComponentByType <PlayerConfigBombermanComponent>
                (*it, BOMBER_PLAYER_CONFIG_COMPONENT);
        assert(playerConfComponent && "PlayerBomberSystem::execSystem flagComponent is null\n");
        if(playerConfComponent->mMode == MODE_PLAYER_NORMAL)
        {
            continue;
        }

        TimerBombermanComponent *timerComponent = stairwayToComponentManager().searchComponentByType <TimerBombermanComponent>
                (*it, BOMBER_TIMER_COMPONENT);
        assert(timerComponent && "PlayerBomberSystem::execSystem flagComponent is null\n");

        if(playerConfComponent->mMode == MODE_PLAYER_DEAD_TRANSITION &&
           timerComponent->mBombClock.getElapsedTime().asMilliseconds() > playerConfComponent->mLatenceBetweenRepop)
        {
            if(playerConfComponent->mNumberLife)
            {
                ecs::PositionComponent *posComp = stairwayToComponentManager().searchComponentByType <ecs::PositionComponent>
                        (*it, ecs::POSITION_COMPONENT);
                assert(posComp && "posComp is null\n");

                MoteurGraphique::static_positionnerCaseTileMap(*posComp, playerConfComponent->mInitX, playerConfComponent->mInitY);
                playerConfComponent->mMode = MODE_PLAYER_AFTER_REPOP;
                //restart chrono for invulnerability frames
                timerComponent->mBombClock.restart();
            }
            //Definitive death
            else
            {
                mptrSystemManager->getptrEngine()->bRmEntity(*it);
            }
        }
        else if(playerConfComponent->mMode == MODE_PLAYER_AFTER_REPOP &&
                timerComponent->mBombClock.getElapsedTime().asMilliseconds() > playerConfComponent->mInvulnerabilityAfterDeath)
        {
            playerConfComponent->mMode = MODE_PLAYER_NORMAL;
        }
    }
}

void PlayerBomberSystem::displaySystem() const
{}

void PlayerBomberSystem::setBombermanDeath(unsigned int numEntity)
{
    PlayerConfigBombermanComponent *playerConfComponent = stairwayToComponentManager() .
            searchComponentByType <PlayerConfigBombermanComponent> (numEntity, BOMBER_PLAYER_CONFIG_COMPONENT);
    if(! playerConfComponent)
    {
        assert(playerConfComponent && "playerConfComponent is null\n");
    }

    if(playerConfComponent->mNumberLife)
    {
        std::cout << "--playerConfComponent->mNumberLife" << "\n";

        --playerConfComponent->mNumberLife;
    }

    playerConfComponent->mMode = MODE_PLAYER_DEAD_TRANSITION;

    TimerBombermanComponent *timerComponent = stairwayToComponentManager().searchComponentByType <TimerBombermanComponent>
            (numEntity, BOMBER_TIMER_COMPONENT);
    assert(timerComponent && "flagComponent is null\n");
    timerComponent->mBombClock.restart();
}
