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
private:
    unsigned int createExplosionEntity();
    bool createExplosions(unsigned int caseX, unsigned int caseY, unsigned int explosionRadius);
    void createEntityExplosion(unsigned int positionCaseX, unsigned int positionCaseY, unsigned int explosionRadius,
                               unsigned int firstSize, unsigned int secondSize, bool vertical);
public:
    ExplosionBombermanSystem();
    virtual void execSystem();
    virtual void displaySystem()const;
    void makeBombExplode(unsigned int numEntityBomb);
};

#endif // EXPLOSIONBOMBERMANSYSTEM_H
