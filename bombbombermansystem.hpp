#ifndef BOMBBOMBERMANSYSTEM_HPP
#define BOMBBOMBERMANSYSTEM_HPP

#include "system.hpp"
#include "constants.hpp"

namespace ecs
{
    struct PositionComponent;
}
class ExplosionBombermanSystem;

class BombBombermanSystem : public ecs::System
{
private:
    unsigned int mTimeExplode;
    ExplosionBombermanSystem *m_ptrExplosionSystem;
public:
    BombBombermanSystem();
    virtual void execSystem();
    virtual void displaySystem()const;
    void lauchBomb(unsigned int numEntity, const ecs::PositionComponent &posA);
};

#endif // BOMBBOMBERMANSYSTEM_HPP
