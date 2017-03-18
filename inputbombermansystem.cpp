#include "inputbombermancomponent.hpp"
#include "inputbombermansystem.hpp"
#include "positioncomponent.hpp"
#include "moveablebombermancomponent.hpp"
#include "constants.hpp"

InputBombermanSystem::InputBombermanSystem()
{
	if( ! bAddComponentToSystem( BOMBER_INPUT_COMPONENT ) )
	{
			std::cout << "Erreur InputSystem ajout BOMBER_INPUT_COMPONENT.\n";
	}
	if( ! bAddComponentToSystem( ecs::POSITION_COMPONENT ) )
	{
			std::cout << "Erreur InputSystem ajout POSITION_COMPONENT.\n";
	}
	if( ! bAddComponentToSystem( BOMBER_MOVEABLE_COMPONENT ) )
	{
			std::cout << "Erreur InputSystem ajout MOVEABLE_COMPONENT.\n";
	}
}

void InputBombermanSystem::execSystem()
{
	System::execSystem();
		for( unsigned int i = 0 ; i < mVectNumEntity.size() ; ++i ){

			InputBombermanComponent * inputComponent = stairwayToComponentManager() .
					searchComponentByType < InputBombermanComponent > ( mVectNumEntity[ i ],
																	BOMBER_INPUT_COMPONENT );
			if( ! inputComponent ){
				std::cout << " InputBombermanSystem pointeur NULL inputComponent \n";
				continue;
			}
			ecs::PositionComponent * positionComp = stairwayToComponentManager() .
					searchComponentByType < ecs::PositionComponent > ( mVectNumEntity[ i ],
																	   ecs::POSITION_COMPONENT );
			if( ! positionComp ){
				std::cout << " InputBombermanSystem pointeur NULL positionComponent \n";
				continue;
			}
			MoveableBombermanComponent * moveableComponent = stairwayToComponentManager() .
					searchComponentByType < MoveableBombermanComponent > ( mVectNumEntity[ i ],
																	   BOMBER_MOVEABLE_COMPONENT );
			if( ! moveableComponent ){
				std::cout << " InputBombermanSystem pointeur NULL moveableComponent \n";
				continue;
			}

			bool bAllFalse = true;
			//si aucune entré utilisateur entité suivante
			for( unsigned int j = 0; j < inputComponent -> mBitsetInput.size();j++ ){
				if( inputComponent -> mBitsetInput[ j ] ){
					bAllFalse = false;
					break;
				}
			}
			if( bAllFalse )continue;

			//traitement évènement joueur

			if( inputComponent -> mBitsetInput[ MOVE_RIGHT ] )positionComp ->
					vect2DPosComp . mfX +=  moveableComponent -> mfVelocite;
			else if( inputComponent -> mBitsetInput[ MOVE_LEFT ] )positionComp ->
					vect2DPosComp . mfX -=  moveableComponent -> mfVelocite;

			if( inputComponent -> mBitsetInput[ MOVE_UP ] )
			{
				positionComp -> vect2DPosComp . mfY -=  moveableComponent -> mfVelocite;
			}
			else if( inputComponent -> mBitsetInput[ MOVE_DOWN ] )positionComp ->
					vect2DPosComp . mfY +=  moveableComponent -> mfVelocite;

			inputComponent -> mBitsetInput . reset();

		}
}

void InputBombermanSystem::displaySystem() const
{

}
