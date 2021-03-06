#ifndef CONSTANTES_HPP
#define CONSTANTES_HPP


namespace ecs
{

//Enumération des types de composants
enum{ DISPLAY_COMPONENT, POSITION_COMPONENT, PHYSIC_COMPONENT, MOVEABLE_COMPONENT, BEHAVIOR_COMPONENT,
	  INPUT_COMPONENT,
	GROUND_COMPONENT, RING_BEHAVIOR_COMPONENT, ROUND_TRIP_BEHAVIOR_COMPONENT,
	  SINUSOID_BEHAVIOR_COMPONENT, COLL_SEGMENT_COMPONENT,
    COLL_RECTBOX_COMPONENT };

//Enumération des types de systemes
enum{ IA_SYSTEM, GRAVITY_SYSTEM, INPUT_SYSTEM, COLLISION_SYSTEM, DISPLAY_SYSTEM  };

//Enumération des commandes joueurs
enum{ MOVE_UP, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, JUMP, ATTACK };

//Enumération des types de comportements
enum{ UNSPECIFIED, SINUSOIDAL, RING, ROUND_TRIP, TOWARD_PLAYER };

//Enumération des tags
enum{ TAG_GROUND, TAG_PLAYER, TAG_PLAYER_PROJECTILE, TAG_ENEMY_PROJECTILE, TAG_ENEMY };

const unsigned int COMPONENT_NOT_FOUND = 1000, SYSTEM_NOT_FOUND = 1001,
NUMBR_SYSTEM_MAX = 16, NUMBR_INPUT = 6,
GROUND_UNDEFINED = 1002, NUM_MIN_COLL_COMPONENT = COLL_SEGMENT_COMPONENT,
NUM_MAX_COLL_COMPONENT = COLL_RECTBOX_COMPONENT + 1, NUMBER_COMPONENT_BASE_ECS = 12,
NUMBER_SYSTEM_BASE_ECS = 5;

const float PI = 3.14159265358979323846;

}//fin namespace
#endif // CONSTANTES_HPP
