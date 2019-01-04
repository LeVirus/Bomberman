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
class PlayerBomberSystem;

class CollisionBombermanSystem: public ecs::System
{
private:
	Tableau2D mTabFlag, mTabInColl;
    bool getComponentForCollision(ecs::PositionComponent *&positionComponent, ecs::CollRectBoxComponent *&collRectBoxComponent,
                                  FlagBombermanComponent *&flagBombermanComponent, MoveableBombermanComponent *&moveableBomberComp,
                                  unsigned int uiNumEntity);

    PlayerBomberSystem *mptrPlayerBomberSystem = nullptr;
    /**
     * @brief checkLimitCollision Vérifie si les collisions sont à une certaine limite.
     * Pour les cas ou l'objet mouvant puisse "glisser" sur l'entité en question.
     * return true si il y a glissement, false sinon.
     */
    bool checkLimitCollision(MoveableBombermanComponent &moveableBomberComp, ecs::CollRectBoxComponent &RectA, ecs::CollRectBoxComponent &RectB, bool vertical);
public:
	CollisionBombermanSystem();
	virtual void execSystem();
	virtual void displaySystem()const;
	bool bCheckFlag(unsigned int flagA, unsigned int flagB);
    void treatBombermanCollisionBehaviorWall(ecs::PositionComponent &posA, MoveableBombermanComponent &moveableBomberComp,
                                         ecs::CollRectBoxComponent &RectA, ecs::CollRectBoxComponent &RectB);
};

#endif // COLLISIONBOMBERMANSYSTEM_HPP
