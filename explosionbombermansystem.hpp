#ifndef EXPLOSIONBOMBERMANSYSTEM_H
#define EXPLOSIONBOMBERMANSYSTEM_H

namespace ecs
{
    struct PositionComponent;
}

#include "system.hpp"

class ExplosionBombermanSystem : public ecs::System
{
public:
    ExplosionBombermanSystem();
    virtual void execSystem();
    virtual void displaySystem()const;
    void makeBombExplode(const ecs::PositionComponent &posComp);
    bool getLimitForExplosion(const unsigned int caseX, const unsigned int caseY,
                              unsigned int &minX, unsigned int &maxX,
                              unsigned int &minY, unsigned int &maxY);
};

#endif // EXPLOSIONBOMBERMANSYSTEM_H
