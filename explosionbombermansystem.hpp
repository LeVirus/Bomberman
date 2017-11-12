#ifndef EXPLOSIONBOMBERMANSYSTEM_H
#define EXPLOSIONBOMBERMANSYSTEM_H

#include "system.hpp"

class ExplosionBombermanSystem : public ecs::System
{
public:
    ExplosionBombermanSystem();
    virtual void execSystem();
    virtual void displaySystem()const;
};

#endif // EXPLOSIONBOMBERMANSYSTEM_H
