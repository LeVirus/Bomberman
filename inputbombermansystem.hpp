#ifndef INPUTBOMBERMANSYSTEM_HPP
#define INPUTBOMBERMANSYSTEM_HPP

#include "system.hpp"

class InputBombermanSystem : public ecs::System
{
public:
	InputBombermanSystem();
	virtual void execSystem();
	virtual void displaySystem()const;
};

#endif // INPUTBOMBERMANSYSTEM_HPP
