#ifndef PLAYERSYSTEM_HPP
#define PLAYERSYSTEM_HPP

#include "system.hpp"
#include "constants.hpp"

class PlayerBomberSystem : public ecs::System
{
public:
    PlayerBomberSystem();
    virtual void execSystem();
    virtual void displaySystem()const;
    void setBombermanDeath(unsigned int numEntity);
};

#endif // PLAYERSYSTEM_HPP
