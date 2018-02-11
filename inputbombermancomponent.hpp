#ifndef INPUTBOMBERMANCOMPONENT_HPP
#define INPUTBOMBERMANCOMPONENT_HPP
#include "component.hpp"
#include "ECSconstantes.hpp"
#include "constants.hpp"
#include <bitset>


struct InputBombermanComponent : public ecs::Component
{
	InputBombermanComponent()
	{
		muiTypeComponent = INPUT_BOMBER_COMPONENT;
	}

	void displayComponent()const override
	{
		//Component::displayComponent();
		std::cout << "muiNumSprite" << muiNumSprite << "\n";
	}
	std::bitset<NUMBR_INPUT> mBitsetInput;
	unsigned int muiNumSprite;
};
#endif // INPUTBOMBERMANCOMPONENT_HPP
