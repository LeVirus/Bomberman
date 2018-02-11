#ifndef FLAGCOMPONENT_HPP
#define FLAGCOMPONENT_HPP

#include "component.hpp"
#include "ECSconstantes.hpp"
#include "constants.hpp"


struct FlagBombermanComponent : public ecs::Component
{
	FlagBombermanComponent()
	{
		muiTypeComponent = FLAG_BOMBER_COMPONENT;
	}

	void displayComponent()const override
	{
		//Component::displayComponent();
		std::cout << "muiNumSprite" << muiNumSprite << "\n";
	}
	unsigned int muiNumSprite, muiNumFlag;
};
#endif // FLAGCOMPONENT_HPP
