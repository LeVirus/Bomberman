#include "collisionbombermansystem.hpp"
#include "collrectboxcomponent.hpp"
#include "moveablebombermancomponent.hpp"
#include "positioncomponent.hpp"
#include "flagcomponent.hpp"
#include "constants.hpp"
#include "vector2D.hpp"
#include "geometriefreefunctions.hpp"

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
            searchComponentByType < FlagBombermanComponent > (uiNumEntity, BOMBER_FLAG_COMPONENT);
    if( ! flagBombermanComponent )
    {
        return false;
    }
    //check if moveable component exists
    moveableBomberComp = stairwayToComponentManager() .
                searchComponentByType < MoveableBombermanComponent > (uiNumEntity, BOMBER_MOVEABLE_COMPONENT);
    return true;
}

CollisionBombermanSystem::CollisionBombermanSystem()
{//FLAG_DESTRUCTIBLE_WALL, FLAG_SOLID_WALL, FLAG_BOMBERMAN, FLAG_EXPLOSION, FLAG_OBJECT, FLAG_BOMB
	std::vector<unsigned char> vect;
	vect.resize(NUMBR_MAX_FLAG * NUMBR_MAX_FLAG);

	vect[NUMBR_MAX_FLAG * 2] = 1;//FLAG_DESTRUCTIBLE_WALL
	vect[NUMBR_MAX_FLAG * 3] = 1;
	vect[NUMBR_MAX_FLAG * 5] = 1;

	vect[NUMBR_MAX_FLAG * 2 + FLAG_SOLID_WALL] = 1;
	vect[NUMBR_MAX_FLAG * 3 + FLAG_SOLID_WALL] = 1;
	vect[NUMBR_MAX_FLAG * 5 + FLAG_SOLID_WALL] = 1;

	vect[FLAG_BOMBERMAN] = 1;
	vect[NUMBR_MAX_FLAG + FLAG_BOMBERMAN] = 1;
	vect[NUMBR_MAX_FLAG * 3 + FLAG_BOMBERMAN] = 1;
	vect[NUMBR_MAX_FLAG * 4 + FLAG_BOMBERMAN] = 1;
	vect[NUMBR_MAX_FLAG * 5 + FLAG_BOMBERMAN] = 1;

	vect[FLAG_EXPLOSION] = 1;
	vect[NUMBR_MAX_FLAG + FLAG_EXPLOSION] = 1;
	vect[NUMBR_MAX_FLAG * 2 + FLAG_EXPLOSION] = 1;
	vect[NUMBR_MAX_FLAG * 5 + FLAG_EXPLOSION] = 1;

	vect[NUMBR_MAX_FLAG * 2 + FLAG_OBJECT] = 1;

	vect[FLAG_BOMB] = 1;
	vect[NUMBR_MAX_FLAG + FLAG_BOMB] = 1;
	vect[NUMBR_MAX_FLAG * 2 + FLAG_BOMB] = 1;
	vect[NUMBR_MAX_FLAG * 3 + FLAG_BOMB] = 1;

	mTabFlag.bAttribuerTab(vect, NUMBR_MAX_FLAG, NUMBR_MAX_FLAG);

	if( ! bAddComponentToSystem( ecs::POSITION_COMPONENT ) )
	{
		std::cout << "Erreur CollisionBombermanSystem ajout POSITION_COMPONENT.\n";
	}
	if( ! bAddComponentToSystem( ecs::COLL_RECTBOX_COMPONENT ) )
	{
		std::cout << "Erreur CollisionBombermanSystem ajout COLL_RECTBOX_COMPONENT.\n";
	}
	if( ! bAddComponentToSystem( BOMBER_FLAG_COMPONENT ) )
	{
		std::cout << "Erreur CollisionBombermanSystem ajout POSITION_COMPONENT.\n";
	}
}

void CollisionBombermanSystem::execSystem()
{
	System::execSystem();
	mTabInColl.resize(mVectNumEntity.size(), mVectNumEntity.size());
	mTabInColl.reset();
	for( unsigned int i = 0 ; i < mVectNumEntity.size() ; ++i ){
        ecs::PositionComponent *positionComponent;
        ecs::CollRectBoxComponent *collRectBoxComponent;
        FlagBombermanComponent *flagBombermanComponent;
        MoveableBombermanComponent *moveableBomberComp;
        if(! getComponentForCollision(positionComponent, collRectBoxComponent, flagBombermanComponent,
                                      moveableBomberComp, mVectNumEntity[i])){
            continue;
        }
        //modifier la position uniquement pour bomberman

        if(i == 0 && moveableBomberComp)
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

            if(i == 0 && moveableBomberCompB)
            {
                collRectBoxComponent->mRectBox.mSetOriginsRectBox(positionComponent->vect2DPosComp +
                                                                  collRectBoxComponent->mVect2dVectOrigins);
            }

            if(! bCheckFlag(flagBombermanComponent->muiNumFlag, flagBombermanComponentB->muiNumFlag))
            {
                continue;
            }
            if(bIsInCollision( collRectBoxComponent->mRectBox, collRectBoxComponentB->mRectBox ))
			{
				mTabInColl.setValAt(  j ,  i , 1 );
				mTabInColl.setValAt(  i ,  j , 1 );
                std::cout << "coll " << i << "  "<< j <<"\n";
                /*std::cout << "beforeAAA\n";
                collRectBoxComponent->mRectBox.mGetOriginsRectBox().displayVector();
				std::cout << "afterAAA\n";
				std::cout << "beforeBBB\n";
                collRectBoxComponentB->mRectBox.mGetOriginsRectBox().displayVector();
				std::cout << "afterBBB\n";
*/
                if(moveableBomberComp)
                {
                    //définis les flags non traversable (inferieur à FLAG_BOMBERMAN (2))
                    if(flagBombermanComponentB->muiNumFlag < FLAG_BOMBERMAN)
                    {
                        treatBombermanCollisionBehavior(*positionComponent, *moveableBomberComp, *collRectBoxComponent, *collRectBoxComponentB);
                    }

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

void CollisionBombermanSystem::treatBombermanCollisionBehavior(ecs::PositionComponent &posA,
                                                               MoveableBombermanComponent &moveableBomberComp,
                                                               ecs::CollRectBoxComponent &RectA,
                                                               ecs::CollRectBoxComponent &RectB)
{
    //MOVE_RIGHT, MOVE_LEFT, MOVE_UP, MOVE_DOWN
    if(moveableBomberComp.mBitsetDirection[MOVE_RIGHT])
    {
        float memSizeBack = RectA.mRectBox.mfGetLenghtRectBox() + RectA.mVect2dVectOrigins.mfX;
        std::cout << "right\n" << "\n";

        //set the new position out of the collision box ==> - 1
        if( abs(posA.vect2DPosComp.mfX - (RectB.mRectBox.mGetOriginsRectBox().mfX - memSizeBack)) <= moveableBomberComp.mfVelocite)
        {
            posA.vect2DPosComp.mfX = RectB.mRectBox.mGetOriginsRectBox().mfX - memSizeBack;
        }
        else
        {
            std::cout << "ha\n";
            //--posA.vect2DPosComp.mfX;
        }
    }
    else if(moveableBomberComp.mBitsetDirection[MOVE_LEFT])
    {
        std::cout << "left\n";

        float newTheoricalPosition = RectB.mRectBox.mGetOriginsRectBox().mfX + RectA.mRectBox.mfGetLenghtRectBox() - RectA.mVect2dVectOrigins.mfX;
        if( abs(posA.vect2DPosComp.mfX - newTheoricalPosition) <= moveableBomberComp.mfVelocite)
        {
            posA.vect2DPosComp.mfX = newTheoricalPosition;
        }
        else
        {
            std::cout << "ha\n";

            //++posA.vect2DPosComp.mfX;
        }
    }
    if(moveableBomberComp.mBitsetDirection[MOVE_UP])
    {
        std::cout << "up\n";
        float newTheoricalPosition = RectB.mRectBox.mGetOriginsRectBox().mfY + RectA.mRectBox.mfGetHeightRectBox() - RectA.mVect2dVectOrigins.mfY;
        if( abs(posA.vect2DPosComp.mfY - newTheoricalPosition) <= moveableBomberComp.mfVelocite)
        {
            posA.vect2DPosComp.mfY = newTheoricalPosition;
        }
    }
    else if(moveableBomberComp.mBitsetDirection[MOVE_DOWN])
    {
        std::cout << "down\n";
        float newTheoricalPosition = RectB.mRectBox.mGetOriginsRectBox().mfY - RectA.mRectBox.mfGetHeightRectBox() + RectA.mVect2dVectOrigins.mfY;
        if( abs(posA.vect2DPosComp.mfY - newTheoricalPosition) <= moveableBomberComp.mfVelocite)
        {
            posA.vect2DPosComp.mfY = newTheoricalPosition;
        }
    }
}
