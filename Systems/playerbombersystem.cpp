#include "playerbombersystem.hpp"
#include "flagcomponent.hpp"
#include "timerbombermancomponent.hpp"
#include "displaycomponent.hpp"
#include "playerconfigbombermancomponent.hpp"
#include "positioncomponent.hpp"
#include "constants.hpp"
#include "ECSconstantes.hpp"
#include "moteurgraphique.hpp"
#include "engine.hpp"
#include <cassert>

PlayerBomberSystem::PlayerBomberSystem()
{
    if(! bAddComponentToSystem(TIMER_BOMBER_COMPONENT))
    {
        std::cout << "Erreur PlayerBomberSystem ajout BOMBER_TIMER_COMPONENT.\n";
    }
    if(! bAddComponentToSystem(PLAYER_CONFIG_BOMBER_COMPONENT))
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
        PlayerConfigBombermanComponent *playerConfComponent = stairwayToComponentManager().searchComponentByType <PlayerConfigBombermanComponent>
                (*it, PLAYER_CONFIG_BOMBER_COMPONENT);
        assert(playerConfComponent && "PlayerBomberSystem::execSystem flagComponent is null\n");
        if(playerConfComponent->mMode == MODE_PLAYER_NORMAL)
        {
            continue;
        }

        TimerBombermanComponent *timerComponent = stairwayToComponentManager().searchComponentByType <TimerBombermanComponent>
                (*it, TIMER_BOMBER_COMPONENT);
        assert(timerComponent && "PlayerBomberSystem::execSystem flagComponent is null\n");
        ecs::DisplayComponent *displayComp = stairwayToComponentManager().searchComponentByType <ecs::DisplayComponent>
                (*it, ecs::DISPLAY_COMPONENT);
        assert(displayComp && "posComp is null\n");

        if(playerConfComponent->mMode == MODE_PLAYER_DEAD_TRANSITION )
        {
            if(displayComp->muiNumSprite < SPRITE_DEAD_A)
            {
                timerComponent->mBombClockC.restart();
                displayComp->muiNumSprite = SPRITE_DEAD_A;
            }
            else if(displayComp->muiNumSprite != SPRITE_DEAD_F && timerComponent->mBombClockC.getElapsedTime().asMilliseconds() > playerConfComponent->mLatenceBetweenRepop / 6)
            {
                ++displayComp->muiNumSprite;
                timerComponent->mBombClockC.restart();

            }
            if(timerComponent->mBombClockB.getElapsedTime().asMilliseconds() > playerConfComponent->mLatenceBetweenRepop)
            {
                if(playerConfComponent->mNumberLife)
                {
                    ecs::PositionComponent *posComp = stairwayToComponentManager().searchComponentByType <ecs::PositionComponent>
                            (*it, ecs::POSITION_COMPONENT);
                    assert(posComp && "posComp is null\n");
                    MoteurGraphique::static_positionnerCaseTileMap(*posComp, playerConfComponent->mInitX, playerConfComponent->mInitY, true);
                    playerConfComponent->mMode = MODE_PLAYER_AFTER_REPOP;
                    //restart chrono for invulnerability frames
                    timerComponent->mBombClockB.restart();
                    timerComponent->mBombClockD.restart();
                    displayComp->muiNumSprite = SPRITE_BOMBERMAN_DOWN_STATIC;
                }
                //Definitive death
                else
                {
                    Niveau::decrementCurrentNumberPlayers();
                    mptrSystemManager->getptrEngine()->bRmEntity(*it);

                }
            }
        }
        else if(playerConfComponent->mMode == MODE_PLAYER_AFTER_REPOP)
        {
            if(timerComponent->mBombClockD.getElapsedTime().asMilliseconds() > 10)
            {
                timerComponent->mBombClockD.restart();
                if(displayComp->mVisible)
                {
                    displayComp->mVisible = false;
                }
                else
                {
                    displayComp->mVisible = true;
                }
            }
            if(timerComponent->mBombClockB.getElapsedTime().asMilliseconds() > playerConfComponent->mInvulnerabilityAfterDeath)
            {
                displayComp->mVisible = true;
                playerConfComponent->mMode = MODE_PLAYER_NORMAL;
            }
        }
    }
}

void PlayerBomberSystem::displaySystem() const
{}

void PlayerBomberSystem::setBombermanDeath(unsigned int numEntity)
{
    PlayerConfigBombermanComponent *playerConfComponent = stairwayToComponentManager() .
            searchComponentByType <PlayerConfigBombermanComponent>(numEntity, PLAYER_CONFIG_BOMBER_COMPONENT);
    if(! playerConfComponent)
    {
        assert(playerConfComponent && "playerConfComponent is null\n");
    }

    if(playerConfComponent->mNumberLife)
    {
        --playerConfComponent->mNumberLife;
    }

    playerConfComponent->mMode = MODE_PLAYER_DEAD_TRANSITION;

    TimerBombermanComponent *timerComponent = stairwayToComponentManager().searchComponentByType<TimerBombermanComponent>
            (numEntity, TIMER_BOMBER_COMPONENT);
    assert(timerComponent && "flagComponent is null\n");
    timerComponent->mBombClockB.restart();
}
