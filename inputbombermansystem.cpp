#include "inputbombermansystem.hpp"
#include "inputcomponent.hpp"
#include "positioncomponent.hpp"
#include "moveablecomponent.hpp"
#include "constants.hpp"

InputBombermanSystem::InputBombermanSystem()
{
	if( ! bAddComponentToSystem( ecs::INPUT_COMPONENT ) )
	{
			std::cout << "Erreur InputSystem ajout INPUT_COMPONENT.\n";
	}
	if( ! bAddComponentToSystem( ecs::POSITION_COMPONENT ) )
	{
			std::cout << "Erreur InputSystem ajout INPUT_COMPONENT.\n";
	}
}

void InputBombermanSystem::execSystem()
{
	System::execSystem();
		for( unsigned int i = 0 ; i < mVectNumEntity.size() ; ++i ){

			ecs::InputComponent * inputComponent = stairwayToComponentManager() .
					searchComponentByType < ecs::InputComponent > ( mVectNumEntity[ i ], ecs::INPUT_COMPONENT );
			if( ! inputComponent ){
				std::cout << " IASystem pointeur NULL inputComponent \n";
				continue;
			}
			ecs::PositionComponent * positionComp = stairwayToComponentManager() .
					searchComponentByType < ecs::PositionComponent > ( mVectNumEntity[ i ], ecs::POSITION_COMPONENT );
			if( ! positionComp ){
				std::cout << " IASystem pointeur NULL positionComponent \n";
				continue;
			}
			ecs::MoveableComponent * moveableComponent = stairwayToComponentManager() .
					searchComponentByType < ecs::MoveableComponent > ( mVectNumEntity[ i ], ecs::MOVEABLE_COMPONENT );
			if( ! moveableComponent ){
				std::cout << " IASystem pointeur NULL moveableComponent \n";
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



			//if( moveableComponent -> mbTerrestrial && moveableComponent -> mbOnTheGround ){
				if( inputComponent -> mBitsetInput[ MOVE_RIGHT ] )positionComp -> vect2DPosComp . mfX +=  moveableComponent -> mfVelocite;
				else if( inputComponent -> mBitsetInput[ MOVE_LEFT ] )positionComp -> vect2DPosComp . mfX -=  moveableComponent -> mfVelocite;
				//JUMP a implémenter
			//}
			//else{
				if( inputComponent -> mBitsetInput[ MOVE_RIGHT ] )positionComp -> vect2DPosComp . mfX +=  moveableComponent -> mfVelocite;
				else if( inputComponent -> mBitsetInput[ MOVE_LEFT ] )positionComp -> vect2DPosComp . mfX -=  moveableComponent -> mfVelocite;

				if( inputComponent -> mBitsetInput[ MOVE_UP ] )positionComp -> vect2DPosComp . mfY -=  moveableComponent -> mfVelocite;
				else if( inputComponent -> mBitsetInput[ MOVE_DOWN ] )positionComp -> vect2DPosComp . mfY +=  moveableComponent -> mfVelocite;
			//}

			//réinitialisation du bitset du composant
			inputComponent -> mBitsetInput . reset();

		}
}

void InputBombermanSystem::displaySystem() const
{

}
