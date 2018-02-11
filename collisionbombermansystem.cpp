#include "collisionbombermansystem.hpp"
#include "collrectboxcomponent.hpp"
#include "moveablebombermancomponent.hpp"
#include "positioncomponent.hpp"
#include "flagcomponent.hpp"
#include "constants.hpp"
#include "vector2D.hpp"
#include "geometriefreefunctions.hpp"
#include "playerconfigbombermancomponent.hpp"
#include "playerbombersystem.hpp"
#include "moteurgraphique.hpp"
#include "engine.hpp"
#include <cassert>

bool CollisionBombermanSystem::getComponentForCollision(ecs::PositionComponent *&positionComponent, ecs::CollRectBoxComponent *&collRectBoxComponent,
                                                        FlagBombermanComponent *&flagBombermanComponent,
                                                        MoveableBombermanComponent *&moveableBomberComp, unsigned int uiNumEntity)
{
    positionComponent = stairwayToComponentManager() .
            searchComponentByType < ecs::PositionComponent > (uiNumEntity, ecs::POSITION_COMPONENT);
    if( ! positionComponent )
    {
        return false;
    }
    collRectBoxComponent = stairwayToComponentManager() .
            searchComponentByType < ecs::CollRectBoxComponent > (uiNumEntity, ecs::COLL_RECTBOX_COMPONENT);
    if( ! collRectBoxComponent )
    {
        return false;
    }

    flagBombermanComponent = stairwayToComponentManager() .
            searchComponentByType < FlagBombermanComponent > (uiNumEntity, FLAG_BOMBER_COMPONENT);
    if( ! flagBombermanComponent )
    {
        return false;
    }
    //check if moveable component exists
    moveableBomberComp = stairwayToComponentManager() .
                searchComponentByType < MoveableBombermanComponent > (uiNumEntity, MOVEABLE_BOMBER_COMPONENT);
    return true;
}

bool CollisionBombermanSystem::checkLimitCollision(MoveableBombermanComponent &moveableBomberComp,
                                                   ecs::CollRectBoxComponent &RectA, ecs::CollRectBoxComponent &RectB, bool vertical)
{
    if(! vertical)
    {
        if(moveableBomberComp.mBitsetDirection[MOVE_UP])
        {
            float limitRectB = RectB.mRectBox.mGetOriginsRectBox().mfY +
                    RectB.mRectBox.mfGetHeightRectBox();
            //limite glissement
            if( abs(RectA.mRectBox.mGetOriginsRectBox().mfY - limitRectB) <= moveableBomberComp.mfVelocite )
            {
                return true;
            }
        }
        if(moveableBomberComp.mBitsetDirection[MOVE_DOWN])
        {
            float limitRectA = (RectA.mRectBox.mfGetHeightRectBox() + RectA.mRectBox.mGetOriginsRectBox().mfY);
            if( abs(RectB.mRectBox.mGetOriginsRectBox().mfY - limitRectA) <= moveableBomberComp.mfVelocite)
            {
                return true;
            }
        }
    }
    //horizontal
    else
    {
        if(moveableBomberComp.mBitsetDirection[MOVE_LEFT])
        {
            float limitRectB = RectB.mRectBox.mfGetLenghtRectBox() + RectB.mRectBox.mGetOriginsRectBox().mfX;
            if(abs(RectA.mRectBox.mGetOriginsRectBox().mfX - limitRectB) <= moveableBomberComp.mfVelocite)
            {
                return true;
            }
        }
        else if(moveableBomberComp.mBitsetDirection[MOVE_RIGHT])
        {
            float limitRectA = RectA.mRectBox.mfGetLenghtRectBox() + RectA.mRectBox.mGetOriginsRectBox().mfX;
            if(abs(RectB.mRectBox.mGetOriginsRectBox().mfX - limitRectA) <= moveableBomberComp.mfVelocite)
            {
                return true;
            }
        }
    }
    return false;

}

CollisionBombermanSystem::CollisionBombermanSystem()
{
    mTabFlag.resize(NUMBR_MAX_FLAG, NUMBR_MAX_FLAG);

    mTabFlag.setValAt(FLAG_DESTRUCTIBLE_WALL, FLAG_BOMBERMAN, 1);
//    mTabFlag.setValAt(FLAG_DESTRUCTIBLE_WALL, FLAG_BOMB, 1);

    mTabFlag.setValAt(FLAG_SOLID_WALL, FLAG_BOMBERMAN, 1);
//    mTabFlag.setValAt(FLAG_SOLID_WALL, FLAG_BOMB, 1);

    mTabFlag.setValAt(FLAG_BOMBERMAN, FLAG_DESTRUCTIBLE_WALL, 1);
    mTabFlag.setValAt(FLAG_BOMBERMAN, FLAG_SOLID_WALL, 1);
    mTabFlag.setValAt(FLAG_BOMBERMAN, FLAG_EXPLOSION, 1);
    mTabFlag.setValAt(FLAG_BOMBERMAN, FLAG_OBJECT, 1);
    //see if bomberman will can throw bomb
//    mTabFlag.setValAt(FLAG_BOMBERMAN, FLAG_BOMB, 1);

    mTabFlag.setValAt(FLAG_EXPLOSION, FLAG_BOMBERMAN, 1);
    //see if explosion make bomb explode in case of collisions
//    mTabFlag.setValAt(FLAG_EXPLOSION, FLAG_BOMB, 1);

    mTabFlag.setValAt(FLAG_OBJECT, FLAG_BOMBERMAN, 1);

//    mTabFlag.setValAt(FLAG_BOMB, FLAG_DESTRUCTIBLE_WALL, 1);
//    mTabFlag.setValAt(FLAG_BOMB, FLAG_SOLID_WALL, 1);
//    mTabFlag.setValAt(FLAG_BOMB, FLAG_BOMBERMAN, 1);
//    mTabFlag.setValAt(FLAG_BOMB, FLAG_EXPLOSION, 1);

    //FLAG_DESTRUCTIBLE_WALL, FLAG_SOLID_WALL, FLAG_BOMBERMAN,
    //FLAG_EXPLOSION, FLAG_OBJECT, FLAG_BOMB

	if( ! bAddComponentToSystem( ecs::POSITION_COMPONENT ) )
	{
		std::cout << "Erreur CollisionBombermanSystem ajout POSITION_COMPONENT.\n";
	}
	if( ! bAddComponentToSystem( ecs::COLL_RECTBOX_COMPONENT ) )
	{
		std::cout << "Erreur CollisionBombermanSystem ajout COLL_RECTBOX_COMPONENT.\n";
	}
    if( ! bAddComponentToSystem( FLAG_BOMBER_COMPONENT ) )
	{
		std::cout << "Erreur CollisionBombermanSystem ajout POSITION_COMPONENT.\n";
	}
}

void CollisionBombermanSystem::execSystem()
{
	System::execSystem();
	mTabInColl.resize(mVectNumEntity.size(), mVectNumEntity.size());
	mTabInColl.reset();
    for(unsigned int i = 0 ; i < mVectNumEntity.size() ; ++i)
    {
        ecs::PositionComponent *positionComponent;
        ecs::CollRectBoxComponent *collRectBoxComponent;
        FlagBombermanComponent *flagBombermanComponent;
        MoveableBombermanComponent *moveableBomberComp;
        if(! getComponentForCollision(positionComponent, collRectBoxComponent, flagBombermanComponent,
                                      moveableBomberComp, mVectNumEntity[i]))
        {
            continue;
        }
        if(flagBombermanComponent->muiNumFlag == FLAG_BOMBERMAN)
        {
            PlayerConfigBombermanComponent *playerConf = stairwayToComponentManager() .
                    searchComponentByType <PlayerConfigBombermanComponent> (mVectNumEntity[i], PLAYER_CONFIG_BOMBER_COMPONENT);
            assert(playerConf && "playerConf null");
            if(playerConf->mMode == MODE_PLAYER_DEAD_TRANSITION)
            {
                continue;
            }
        }
        //modifier la position uniquement pour bomberman
        if(moveableBomberComp)
        {
            collRectBoxComponent->mRectBox.mSetOriginsRectBox(positionComponent->vect2DPosComp +
                                                              collRectBoxComponent->mVect2dVectOrigins);
        }
		for( unsigned int j = i + 1; j < mVectNumEntity.size(); ++j )
		{
            ecs::PositionComponent *positionComponentB;
            ecs::CollRectBoxComponent *collRectBoxComponentB;
            FlagBombermanComponent *flagBombermanComponentB;
            MoveableBombermanComponent *moveableBomberCompB;
            if(! getComponentForCollision(positionComponentB, collRectBoxComponentB,
                                          flagBombermanComponentB, moveableBomberCompB, mVectNumEntity[j]))
            {
                continue;
            }
            if(! bCheckFlag(flagBombermanComponent->muiNumFlag, flagBombermanComponentB->muiNumFlag))
            {
                continue;
            }
            if(bIsInCollision(collRectBoxComponent->mRectBox, collRectBoxComponentB->mRectBox))
			{
                mTabInColl.setValAt(j,  i, 1);
                mTabInColl.setValAt(i,  j, 1);
                if(moveableBomberComp)
                {
                    //définis les flags non traversable (inferieur à FLAG_BOMBERMAN (2))
                    if(flagBombermanComponentB->muiNumFlag < FLAG_BOMBERMAN)
                    {
                        treatBombermanCollisionBehaviorWall(*positionComponent, *moveableBomberComp, *collRectBoxComponent, *collRectBoxComponentB);
                    }
                }
                //!!!Check if bomberman entities are at the beginning on the array!!!
                if(flagBombermanComponent->muiNumFlag == FLAG_BOMBERMAN && flagBombermanComponentB->muiNumFlag == FLAG_EXPLOSION)
                {
                    PlayerConfigBombermanComponent *playerConf = stairwayToComponentManager() .
                            searchComponentByType <PlayerConfigBombermanComponent> (mVectNumEntity[i], PLAYER_CONFIG_BOMBER_COMPONENT);
                    assert(playerConf && "playerConf null");
                    if(playerConf->mMode == MODE_PLAYER_AFTER_REPOP)
                    {
                        continue;
                    }
                    if(! mptrPlayerBomberSystem)
                    {
                        mptrPlayerBomberSystem = mptrSystemManager->searchSystemByType<PlayerBomberSystem>(PLAYER_BOMBER_SYSTEM);
                        assert(mptrPlayerBomberSystem && "PlayerBomberSystem is null\n");
                    }
                    mptrPlayerBomberSystem->setBombermanDeath(mVectNumEntity[i]);
                    break;
                }
			}
		}
	}
}

void CollisionBombermanSystem::displaySystem() const
{

}

bool CollisionBombermanSystem::bCheckFlag(unsigned int flagA, unsigned int flagB)
{
    return mTabFlag.getValAt(flagA, flagB);
}

void CollisionBombermanSystem::treatBombermanCollisionBehaviorWall(ecs::PositionComponent &posA,
                                                               MoveableBombermanComponent &moveableBomberComp,
                                                               ecs::CollRectBoxComponent &RectA,
                                                               ecs::CollRectBoxComponent &RectB)
{
    if(moveableBomberComp.mBitsetDirection[MOVE_RIGHT])
    {
        //set the new position out of the collision box ==> - 1
        //+1 quick fix
        if(! checkLimitCollision(moveableBomberComp, RectA, RectB, false))
        {
            float newTheoricalPosition = RectB.mRectBox.mGetOriginsRectBox().mfX -
                    RectA.mRectBox.mfGetLenghtRectBox() - RectA.mVect2dVectOrigins.mfX - 2;
            if( abs(posA.vect2DPosComp.mfX -  newTheoricalPosition) <= moveableBomberComp.mfVelocite + 1)
            {
                posA.vect2DPosComp.mfX = newTheoricalPosition;
            }
        }
    }
    else if(moveableBomberComp.mBitsetDirection[MOVE_LEFT])
    {
        if(! checkLimitCollision(moveableBomberComp, RectA, RectB, false))
        {
            float newTheoricalPosition = RectB.mRectBox.mGetOriginsRectBox().mfX + RectB.mRectBox.mfGetLenghtRectBox() -
                    RectA.mVect2dVectOrigins.mfX + 2;
            if( abs(posA.vect2DPosComp.mfX - newTheoricalPosition) <= moveableBomberComp.mfVelocite /*+ 1*/)
            {
                posA.vect2DPosComp.mfX = newTheoricalPosition;
            }
         }
    }

    if(moveableBomberComp.mBitsetDirection[MOVE_UP])
    {
        if(! checkLimitCollision(moveableBomberComp, RectA, RectB, true))
        {
            float newTheoricalPosition = RectB.mRectBox.mGetOriginsRectBox().mfY +
                    RectB.mRectBox.mfGetHeightRectBox() - RectA.mVect2dVectOrigins.mfY + 2;
            if( abs(posA.vect2DPosComp.mfY - newTheoricalPosition) <= moveableBomberComp.mfVelocite /*+ 1*/)
            {
                posA.vect2DPosComp.mfY = newTheoricalPosition;
            }
        }

    }
    else if(moveableBomberComp.mBitsetDirection[MOVE_DOWN])
    {
        if(! checkLimitCollision(moveableBomberComp, RectA, RectB, true))
        {
            float newTheoricalPosition = RectB.mRectBox.mGetOriginsRectBox().mfY -
                    (RectA.mRectBox.mfGetHeightRectBox() + RectA.mVect2dVectOrigins.mfY) - 2;
            if( abs(posA.vect2DPosComp.mfY - newTheoricalPosition) <= moveableBomberComp.mfVelocite /*+ 1*/)
            {
                posA.vect2DPosComp.mfY = newTheoricalPosition;
            }
        }
    }
}
