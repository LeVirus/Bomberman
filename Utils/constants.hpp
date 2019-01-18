#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "ECSconstantes.hpp"

const unsigned int WIDTH_SCREEN = 1920, HEIGHT_SCREEN = 1080,
MAX_PLAYER = 4, POSITION_LEVEL_X = 180,POSITION_LEVEL_Y = 70, NUMBR_INPUT = 8,
NUMBR_MAX_FLAG = 6;

const float SIZE_SCALE = 5.0f;
const unsigned int INPUT_BOMBER_COMPONENT = ecs::NUMBER_COMPONENT_BASE_ECS,
MOVEABLE_BOMBER_COMPONENT = ecs::NUMBER_COMPONENT_BASE_ECS + 1,
FLAG_BOMBER_COMPONENT = ecs::NUMBER_COMPONENT_BASE_ECS + 2,
TIMER_BOMBER_COMPONENT = ecs::NUMBER_COMPONENT_BASE_ECS + 3,
TILEMAP_BOMBER_COMPONENT = ecs::NUMBER_COMPONENT_BASE_ECS + 4,
PLAYER_CONFIG_BOMBER_COMPONENT = ecs::NUMBER_COMPONENT_BASE_ECS + 5,
BOMB_CONFIG_BOMBER_COMPONENT = ecs::NUMBER_COMPONENT_BASE_ECS + 6,
NETWORK_BOMBER_COMPONENT = ecs::NUMBER_COMPONENT_BASE_ECS + 7;

const unsigned int INPUT_BOMBER_SYSTEM = ecs::NUMBER_SYSTEM_BASE_ECS,
COLLISION_BOMBER_SYSTEM = ecs::NUMBER_SYSTEM_BASE_ECS + 1,
EXPLOSION_BOMBER_SYSTEM = ecs::NUMBER_SYSTEM_BASE_ECS + 2,
BOMB_BOMBER_SYSTEM = ecs::NUMBER_SYSTEM_BASE_ECS + 3,
TILEMAP_BOMBER_SYSTEM = ecs::NUMBER_SYSTEM_BASE_ECS + 4,
PLAYER_BOMBER_SYSTEM = ecs::NUMBER_SYSTEM_BASE_ECS + 5,
SOCKET_SYSTEM = ecs::NUMBER_SYSTEM_BASE_ECS + 6;

const unsigned int SOCKET_DATA_SIZE = 500;


const unsigned int SPRITE_INVISIBLE = 5000;
enum
{
    TEXTURE_BOMBERMAN, TEXTURE_EXPLOSION, TEXTURE_LEVEL
};

enum GameMode
{
    SERVER, CLIENT, SOLO
};

enum
{
    FLAG_SOLID_WALL, FLAG_DESTRUCTIBLE_WALL, FLAG_BOMBERMAN, FLAG_EXPLOSION, FLAG_OBJECT, FLAG_BOMB
};

enum
{
    TILE_SOLID_WALL_EXTERN, TILE_SOLID_WALL_INTERN, TILE_DESTRUCTIBLE_WALL, TILE_EMPTY,
    TILE_DESTRUCTION_WALL_A, TILE_DESTRUCTION_WALL_B, TILE_DESTRUCTION_WALL_C,
    TILE_DESTRUCTION_WALL_D, TILE_DESTRUCTION_WALL_E, TILE_DESTRUCTION_WALL_F
};

enum
{
    MOVE_RIGHT, MOVE_LEFT, MOVE_UP, MOVE_DOWN, MOVE_LEFT_UP, MOVE_LEFT_DOWN, MOVE_RIGHT_DOWN, MOVE_RIGHT_UP, LAUNCH_BOMB
};

enum
{
    SPRITE_BOMBERMAN_UP_STEPA, SPRITE_BOMBERMAN_UP_STATIC, SPRITE_BOMBERMAN_UP_STEPB,
    SPRITE_BOMBERMAN_LEFT_STEPA, SPRITE_BOMBERMAN_LEFT_STATIC, SPRITE_BOMBERMAN_LEFT_STEPB,
    SPRITE_BOMBERMAN_DOWN_STEPA, SPRITE_BOMBERMAN_DOWN_STATIC, SPRITE_BOMBERMAN_DOWN_STEPB,
    SPRITE_BOMBERMAN_RIGHT_STEPA, SPRITE_BOMBERMAN_RIGHT_STATIC, SPRITE_BOMBERMAN_RIGHT_STEPB,
    SPRITE_DEAD_A, SPRITE_DEAD_B, SPRITE_DEAD_C, SPRITE_DEAD_D, SPRITE_DEAD_E, SPRITE_DEAD_F,
    SPRITE_BOMB_LITTLE, SPRITE_BOMB_MEDIUM, SPRITE_BOMB_BIG
};

enum
{
    EXPLOSION_END_LEFT, EXPLOSION_END_RIGHT, EXPLOSION_CENTER, EXPLOSION_VERTICAL_MIDDLE,
    EXPLOSION_END_UP, EXPLOSION_END_DOWN, EXPLOSION_HORIZONTAL_MIDDLE,
    EXPLOSION_END_LEFT_B, EXPLOSION_END_RIGHT_B, EXPLOSION_CENTER_B, EXPLOSION_VERTICAL_MIDDLE_B,
    EXPLOSION_END_UP_B, EXPLOSION_END_DOWN_B, EXPLOSION_HORIZONTAL_MIDDLE_B,
    EXPLOSION_END_LEFT_C, EXPLOSION_END_RIGHT_C, EXPLOSION_CENTER_C, EXPLOSION_VERTICAL_MIDDLE_C,
    EXPLOSION_END_UP_C, EXPLOSION_END_DOWN_C, EXPLOSION_HORIZONTAL_MIDDLE_C,
    EXPLOSION_END_LEFT_D, EXPLOSION_END_RIGHT_D, EXPLOSION_CENTER_D, EXPLOSION_VERTICAL_MIDDLE_D,
    EXPLOSION_END_UP_D, EXPLOSION_END_DOWN_D, EXPLOSION_HORIZONTAL_MIDDLE_D,
    EXPLOSION_END_LEFT_E, EXPLOSION_END_RIGHT_E, EXPLOSION_CENTER_E, EXPLOSION_VERTICAL_MIDDLE_E,
    EXPLOSION_END_UP_E, EXPLOSION_END_DOWN_E, EXPLOSION_HORIZONTAL_MIDDLE_E
};

enum
{
    MODE_PLAYER_NORMAL, MODE_PLAYER_DEAD_TRANSITION, MODE_PLAYER_AFTER_REPOP
};

enum
{
    INPUT_PLAYER_A, INPUT_PLAYER_B, INPUT_PLAYER_C, INPUT_PLAYER_D
};

enum NetworkTypeEntity
{
    TYPE_BOMBERMAN, TYPE_BOMB, TYPE_DESTRUCTIVE_WALL
};

#endif // CONSTANTS_HPP
