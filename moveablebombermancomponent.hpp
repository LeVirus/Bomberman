#ifndef MOVEABLEBOMBERMANCOMPONENT_HPP
#define MOVEABLEBOMBERMANCOMPONENT_HPP

#include "component.hpp"
#include "constants.hpp"
#include <bitset>

struct MoveableBombermanComponent : public ecs::Component
{
	float mfVelocite = 10;
	bool moveInProgress = false;
	unsigned int muiDirection;
    std::bitset<4> mBitsetDirection;

	MoveableBombermanComponent()//: muiTypeComponent(BOMBER_MOVEABLE_COMPONENT)
	{
		muiTypeComponent = BOMBER_MOVEABLE_COMPONENT;
	}

	void displayComponent()const override{
		std::cout << "\n mfVelocite" << mfVelocite << "\n";
	}
};

#endif // MOVEABLEBOMBERMANCOMPONENT_HPP
