#ifndef INPUTBOMBERMANSYSTEM_HPP
#define INPUTBOMBERMANSYSTEM_HPP

#include "system.hpp"
#include "constants.hpp"

namespace ecs
{
    struct DisplayComponent;
}

class InputBombermanSystem : public ecs::System
{
private:
    void setMoveableDirection(std::bitset<4> &directionComp, const std::bitset<NUMBR_INPUT> &inputComp);
    void setSpriteForBomberman(unsigned int direction, ecs::DisplayComponent &displayComp, bool &previousStepA);
public:
	InputBombermanSystem();
	virtual void execSystem();
	virtual void displaySystem()const;
};

#endif // INPUTBOMBERMANSYSTEM_HPP
