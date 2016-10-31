#ifndef GESTIONNAIREECS_H
#define GESTIONNAIREECS_H

#include "engine.hpp"
#include <bitset>


class GestionnaireECS
{
private:
	ecs::Engine mEngineECS;
	ecs::ComponentManager* mCompMan = nullptr;
	ecs::SystemManager* mSysMan = nullptr;
public:
	GestionnaireECS();
	unsigned int addEntity( const std::bitset<ecs::NUMBR_COMPONENT> &bitsetComp );
};

#endif // GESTIONNAIREECS_H
