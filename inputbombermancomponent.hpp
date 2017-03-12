#ifndef INPUTBOMBERMANCOMPONENT_HPP
#define INPUTBOMBERMANCOMPONENT_HPP
#include "component.hpp"
#include "ECSconstantes.hpp"
#include "constants.hpp"


struct InputBombermanComponent : public Component
{
	InputBombermanComponent()
	{
		muiTypeComponent = BOMBER_INPUT_COMPONENT;
	}

	void displayComponent()const override
	{
		//Component::displayComponent();
		std::cout << "muiNumSprite" << muiNumSprite << "\n";
	}
	unsigned int muiNumSprite;
	bool bInMovement = false;
};
#endif // INPUTBOMBERMANCOMPONENT_HPP
