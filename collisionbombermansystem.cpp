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
    bool bMoveableCompA, bMoveableCompB;
	System::execSystem();
	mTabInColl.resize(mVectNumEntity.size(), mVectNumEntity.size());
	mTabInColl.reset();
	for( unsigned int i = 0 ; i < mVectNumEntity.size() ; ++i ){
        bMoveableCompA = false;
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
            bMoveableCompA = true;
            collRectBoxComponent->mRectBox.mSetOriginsRectBox(positionComponent->vect2DPosComp +
                                                              collRectBoxComponent->mVect2dVectOrigins);
        }
		for( unsigned int j = i + 1; j < mVectNumEntity.size(); ++j )
		{
            bMoveableCompB = false;
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
                bMoveableCompB = true;
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
                if(flagBombermanComponent->muiNumFlag == FLAG_BOMBERMAN)
                {
                    //définis les flags non traversable (inferieur à FLAG_BOMBERMAN (2))
                    if(flagBombermanComponentB->muiNumFlag < FLAG_BOMBERMAN)
                    {
                        treatBombermanCollisionBehavior(*positionComponent, *collRectBoxComponent, *collRectBoxComponentB);
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

bool CollisionBombermanSystem::treatBombermanCollisionBehavior(ecs::PositionComponent &posA, const ecs::CollRectBoxComponent &RectA,
                                                               const ecs::CollRectBoxComponent &RectB)
{

}
