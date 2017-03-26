#include "collisionbombermansystem.hpp"
#include "collrectboxcomponent.hpp"
#include "positioncomponent.hpp"
#include "flagcomponent.hpp"
#include "constants.hpp"
#include "vector2D.hpp"
#include "geometriefreefunctions.hpp"

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
		ecs::PositionComponent * positionComponent = stairwayToComponentManager() .
				searchComponentByType < ecs::PositionComponent > ( mVectNumEntity[ i ],
																   ecs::POSITION_COMPONENT );
		if( ! positionComponent )
		{
			continue;
		}
		ecs::CollRectBoxComponent* collRectBoxComponent = stairwayToComponentManager() .
				searchComponentByType < ecs::CollRectBoxComponent > ( mVectNumEntity[ i ],
																	  ecs::COLL_RECTBOX_COMPONENT );
		if( ! collRectBoxComponent )
		{
			continue;
		}
		FlagBombermanComponent * flagBombermanComponent = stairwayToComponentManager() .
				searchComponentByType < FlagBombermanComponent > ( mVectNumEntity[ i ],
																   BOMBER_FLAG_COMPONENT );
		if( ! flagBombermanComponent )
		{
			continue;
		}

		if( i == 0 )
		{
			collRectBoxComponent->mRectBox.mSetOriginsRectBox(positionComponent->vect2DPosComp +
															  collRectBoxComponent->mVect2dVectOrigins);
		}

		std::cout << "beforeAAA\n";
		collRectBoxComponent->mRectBox.mGetOriginsRectBox().displayVector();
		std::cout << "beforddddddddddddddddddd\n";
		//loop check collision
		for( unsigned int j = i + 1; j < mVectNumEntity.size(); ++j )
		{

			FlagBombermanComponent * flagBombermanComponentB = stairwayToComponentManager() .
					searchComponentByType < FlagBombermanComponent > ( mVectNumEntity[ j ],
																	   BOMBER_FLAG_COMPONENT );

			if(!flagBombermanComponentB || !mTabFlag.getValAt(
						flagBombermanComponent->muiNumFlag, flagBombermanComponentB->muiNumFlag ))
			{
				continue;
			}

			ecs::CollRectBoxComponent* collRectBoxComponentB = stairwayToComponentManager() .
					searchComponentByType < ecs::CollRectBoxComponent > ( mVectNumEntity[ j ],
																		  ecs::COLL_RECTBOX_COMPONENT );
			if( ! collRectBoxComponentB )
			{
				continue;
			}

			ecs::PositionComponent * positionComponentB = stairwayToComponentManager() .
					searchComponentByType < ecs::PositionComponent > ( mVectNumEntity[ j ],
																	   ecs::POSITION_COMPONENT );
			if( ! positionComponentB )
			{
				continue;
			}
			if( i == 0 )
			{
				collRectBoxComponentB->mRectBox.mSetOriginsRectBox(
							positionComponentB->vect2DPosComp + collRectBoxComponentB->mVect2dVectOrigins);
			}

			std::cout << "beforeBBB\n";
			collRectBoxComponentB->mRectBox.mGetOriginsRectBox().displayVector();
			std::cout << "beforddddddddddddddddddd\n";



			if(bIsInCollision( collRectBoxComponent->mRectBox, collRectBoxComponentB->mRectBox ))
			{
				mTabInColl.setValAt(  j ,  i , 1 );
				mTabInColl.setValAt(  i ,  j , 1 );
				mTabInColl.afficherTab();
			}
		}

	}
	mTabInColl.afficherTab();

}

void CollisionBombermanSystem::displaySystem() const
{

}
