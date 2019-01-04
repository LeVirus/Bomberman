#include "inputbombermansystem.hpp"
#include "bombbombermansystem.hpp"
#include "positioncomponent.hpp"
#include "playerconfigbombermancomponent.hpp"
#include "inputbombermancomponent.hpp"
#include "timerbombermancomponent.hpp"
#include "moveablebombermancomponent.hpp"
#include "constants.hpp"
#include "flagcomponent.hpp"
#include "displaycomponent.hpp"

InputBombermanSystem::InputBombermanSystem()
{
    if(! bAddComponentToSystem(INPUT_BOMBER_COMPONENT))
	{
			std::cout << "Erreur InputSystem ajout BOMBER_INPUT_COMPONENT.\n";
	}
    if(! bAddComponentToSystem(ecs::POSITION_COMPONENT))
	{
			std::cout << "Erreur InputSystem ajout POSITION_COMPONENT.\n";
	}
    if(! bAddComponentToSystem(MOVEABLE_BOMBER_COMPONENT))
	{
            std::cout << "Erreur InputSystem ajout BOMBER_MOVEABLE_COMPONENT.\n";
	}
}


void InputBombermanSystem::setMoveableDirection(std::bitset<4> &directionComp, const std::bitset<NUMBR_INPUT> &inputComp)
{
    directionComp.reset();
    if(inputComp[MOVE_UP])
    {
        directionComp[MOVE_UP] = true;
    }
    else if(inputComp[MOVE_DOWN])
    {
        directionComp[MOVE_DOWN] = true;
    }
    if(inputComp[MOVE_RIGHT])
    {
        directionComp[MOVE_RIGHT] = true;
    }
    else if(inputComp[MOVE_LEFT])
    {
        directionComp[MOVE_LEFT] = true;
    }
}

void InputBombermanSystem::setSpriteForBomberman(unsigned int direction, ecs::DisplayComponent &displayComp, bool &previousStepA)
{
    unsigned int &memCurrentSprite = displayComp.muiNumSprite;

    unsigned int memStatic, memStepA, memStepB;
    switch (direction) {
    case MOVE_DOWN:
        memStatic = SPRITE_BOMBERMAN_DOWN_STATIC;
        memStepA = SPRITE_BOMBERMAN_DOWN_STEPA;
        memStepB = SPRITE_BOMBERMAN_DOWN_STEPB;
        break;
    case MOVE_UP:
        memStatic = SPRITE_BOMBERMAN_UP_STATIC;
        memStepA = SPRITE_BOMBERMAN_UP_STEPA;
        memStepB = SPRITE_BOMBERMAN_UP_STEPB;
        break;
    case MOVE_LEFT:
        memStatic = SPRITE_BOMBERMAN_LEFT_STATIC;
        memStepA = SPRITE_BOMBERMAN_LEFT_STEPA;
        memStepB = SPRITE_BOMBERMAN_LEFT_STEPB;
        break;
    case MOVE_RIGHT:
        memStatic = SPRITE_BOMBERMAN_RIGHT_STATIC;
        memStepA = SPRITE_BOMBERMAN_RIGHT_STEPA;
        memStepB = SPRITE_BOMBERMAN_RIGHT_STEPB;

        break;
    default:
        return;
    }
    if(memCurrentSprite == memStatic)
    {
        if(previousStepA)memCurrentSprite = memStepB;
        else memCurrentSprite = memStepA;
        if(previousStepA)previousStepA = false;//WTF previousStepA = !previousStepA don't fucking
        else previousStepA = true;        }
    else if(memCurrentSprite == memStepA || memCurrentSprite == memStepB)
    {
        memCurrentSprite = memStatic;
    }
    else
    {
        memCurrentSprite = memStepA;
    }
}

void InputBombermanSystem::execSystem()
{
	System::execSystem();
        for(unsigned int i = 0 ; i < mVectNumEntity.size() ; ++i)
        {
            PlayerConfigBombermanComponent *playerConfigComp = stairwayToComponentManager() .
                    searchComponentByType<PlayerConfigBombermanComponent>(mVectNumEntity[i],
                                                                       PLAYER_CONFIG_BOMBER_COMPONENT);
            if(! playerConfigComp || playerConfigComp->mMode == MODE_PLAYER_DEAD_TRANSITION)
            {
                continue;
            }
            InputBombermanComponent *inputComponent = stairwayToComponentManager() .
                    searchComponentByType<InputBombermanComponent>(mVectNumEntity[i],
                                                                    INPUT_BOMBER_COMPONENT);
            if(! inputComponent)
            {
                std::cout << i << " InputBombermanSystem pointeur NULL inputComponent \n";
				continue;
			}
            ecs::PositionComponent *positionComp = stairwayToComponentManager() .
                    searchComponentByType<ecs::PositionComponent>(mVectNumEntity[i],
                                                                       ecs::POSITION_COMPONENT);
            if(! positionComp)
            {
				std::cout << " InputBombermanSystem pointeur NULL positionComponent \n";
				continue;
			}
            MoveableBombermanComponent *moveableComponent = stairwayToComponentManager() .
                    searchComponentByType<MoveableBombermanComponent>(mVectNumEntity[i],
                                                                       MOVEABLE_BOMBER_COMPONENT);
            if(! moveableComponent)
            {
				std::cout << " InputBombermanSystem pointeur NULL moveableComponent \n";
				continue;
			}

            TimerBombermanComponent *timerComp = stairwayToComponentManager() .
                    searchComponentByType<TimerBombermanComponent>(mVectNumEntity[i],
                                                                       TIMER_BOMBER_COMPONENT);
            if(! timerComp)
            {
                std::cout << " TimerBombermanComponent pointeur NULL playerConfigComp \n";
                continue;
            }

            ecs::DisplayComponent *displayComp = stairwayToComponentManager() .
                    searchComponentByType<ecs::DisplayComponent>(mVectNumEntity[i], ecs::DISPLAY_COMPONENT);
            if(! displayComp)
            {
                std::cout << " displayComp pointeur NULL playerConfigComp \n";
                continue;
            }

			//si aucune entré utilisateur entité suivante
            if(inputComponent -> mBitsetInput.none())continue;

			//traitement évènement joueur
            setMoveableDirection(moveableComponent->mBitsetDirection, inputComponent->mBitsetInput);
            bool changeSprite = false;

            if(timerComp->mBombClockC.getElapsedTime().asMilliseconds() > 200)
            {
                changeSprite =true;
            }

            if(inputComponent->mBitsetInput[MOVE_RIGHT])
            {
                positionComp->vect2DPosComp.mfX += moveableComponent->mfVelocite;
                if(changeSprite)
                {
                    setSpriteForBomberman(MOVE_RIGHT, *displayComp, playerConfigComp->mPreviousStepA);
                    timerComp->mBombClockC.restart();
                    changeSprite = false;
                }
            }
            else if(inputComponent->mBitsetInput[MOVE_LEFT])
            {
                positionComp->vect2DPosComp.mfX -= moveableComponent->mfVelocite;
                if(changeSprite)
                {
                    setSpriteForBomberman(MOVE_LEFT, *displayComp, playerConfigComp->mPreviousStepA);
                    timerComp->mBombClockC.restart();
                    changeSprite = false;
                }
            }

            if(inputComponent->mBitsetInput[MOVE_UP])
            {
                positionComp->vect2DPosComp.mfY -=  moveableComponent->mfVelocite;
                if(changeSprite)
                {
                    setSpriteForBomberman(MOVE_UP, *displayComp, playerConfigComp->mPreviousStepA);
                    timerComp->mBombClockC.restart();
                }
            }
            else if(inputComponent->mBitsetInput[MOVE_DOWN])
            {
                positionComp->vect2DPosComp.mfY +=  moveableComponent->mfVelocite;
                if(changeSprite)
                {
                    setSpriteForBomberman(MOVE_DOWN, *displayComp, playerConfigComp->mPreviousStepA);
                    timerComp->mBombClockC.restart();
                }
            }
            if(timerComp->mLaunched && timerComp->mBombClock.getElapsedTime().asMilliseconds() > playerConfigComp->mLatenceBetweenBomb)
            {
                timerComp->mLaunched = false;
            }
            if(inputComponent->mBitsetInput[LAUNCH_BOMB] && ! timerComp->mLaunched)
            {
                BombBombermanSystem *bombSystem = mptrSystemManager->searchSystemByType
                                                  <BombBombermanSystem>( BOMB_BOMBER_SYSTEM );
                //send notification to bomb system
                bombSystem->lauchBomb(mVectNumEntity[i], *positionComp);
                timerComp->mBombClock.restart();
                timerComp->mLaunched = true;

            }
            inputComponent->mBitsetInput.reset();
		}
}

void InputBombermanSystem::displaySystem() const
{}
