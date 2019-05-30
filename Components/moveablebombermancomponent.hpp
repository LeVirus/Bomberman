#ifndef MOVEABLEBOMBERMANCOMPONENT_HPP
#define MOVEABLEBOMBERMANCOMPONENT_HPP

#include "component.hpp"
#include "constants.hpp"
#include <bitset>

struct MoveableBombermanComponent : public ecs::Component
{
    float mfVelocite = 6;
	bool moveInProgress = false;
    std::bitset<4> mBitsetDirection;

	MoveableBombermanComponent()//: muiTypeComponent(BOMBER_MOVEABLE_COMPONENT)
	{
		muiTypeComponent = MOVEABLE_BOMBER_COMPONENT;
	}

	void displayComponent()const override{
        std::cout << "\n mfVelocite : " << mfVelocite << "\n";
	}
};

#endif // MOVEABLEBOMBERMANCOMPONENT_HPP
