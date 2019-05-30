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
		std::cout << "muiNumSprite" << muiNumSprite << "\n";
	}
	unsigned int muiNumSprite, muiNumFlag;
    virtual ~FlagBombermanComponent() = default;
};
#endif // FLAGCOMPONENT_HPP
