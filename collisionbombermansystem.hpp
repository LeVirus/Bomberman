#ifndef COLLISIONBOMBERMANSYSTEM_HPP
#define COLLISIONBOMBERMANSYSTEM_HPP

#include "system.hpp"
#include "tableau2d.hpp"


class CollisionBombermanSystem: public ecs::System
{
private:
	Tableau2D mTabFlag, mTabInColl;
public:
	CollisionBombermanSystem();
	virtual void execSystem();
	virtual void displaySystem()const;
};

#endif // COLLISIONBOMBERMANSYSTEM_HPP
