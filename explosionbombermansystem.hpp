#ifndef EXPLOSIONBOMBERMANSYSTEM_H
#define EXPLOSIONBOMBERMANSYSTEM_H

namespace ecs
{
    struct PositionComponent;
}

struct TilemapBombermanComponent;

#include "system.hpp"

class ExplosionBombermanSystem : public ecs::System
{
public:
    ExplosionBombermanSystem();
    virtual void execSystem();
    virtual void displaySystem()const;
    void makeBombExplode(unsigned int numEntityBomb);
    unsigned int createExplosionEntity();
    bool createExplosions(unsigned int caseX, unsigned int caseY, unsigned int explosionRadius);
};

#endif // EXPLOSIONBOMBERMANSYSTEM_H
