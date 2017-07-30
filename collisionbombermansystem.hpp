#ifndef COLLISIONBOMBERMANSYSTEM_HPP
#define COLLISIONBOMBERMANSYSTEM_HPP

#include "system.hpp"
#include "tableau2d.hpp"

namespace ecs
{
    struct PositionComponent;
    struct CollRectBoxComponent;
}

struct FlagBombermanComponent;
struct MoveableBombermanComponent;

class CollisionBombermanSystem: public ecs::System
{
private:
	Tableau2D mTabFlag, mTabInColl;
    bool getComponentForCollision(ecs::PositionComponent *&positionComponent, ecs::CollRectBoxComponent *&collRectBoxComponent,
                                  FlagBombermanComponent *&flagBombermanComponent, MoveableBombermanComponent *&moveableBomberComp,
                                  unsigned int uiNumEntity);
public:
	CollisionBombermanSystem();
	virtual void execSystem();
	virtual void displaySystem()const;
	bool bCheckFlag(unsigned int flagA, unsigned int flagB);
    bool treatBombermanCollisionBehavior(ecs::PositionComponent &posA, const ecs::CollRectBoxComponent &RectA,
                                         const ecs::CollRectBoxComponent &RectB);
};

#endif // COLLISIONBOMBERMANSYSTEM_HPP
