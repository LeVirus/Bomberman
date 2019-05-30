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
    ExplosionBombermanSystem *m_ptrExplosionSystem = nullptr;
    unsigned int createBombEntity(bool network = false);
    void changeSpriteBomb(unsigned int numEntity, bool &previousSizeLittle);
public:
    BombBombermanSystem();
    virtual void execSystem();
    virtual void displaySystem()const;
    void lauchBomb(unsigned int numEntity, const ecs::PositionComponent &posA, bool network = false);
};

#endif // BOMBBOMBERMANSYSTEM_HPP
