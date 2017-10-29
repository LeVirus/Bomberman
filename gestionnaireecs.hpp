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
	ecs::SystemManager *getECSSystemManager();
	ecs::ComponentManager *getECSComponentManager();
    ecs::Engine *getECSEngine();
    void initECS();
	unsigned int addEntity( const std::vector< bool > &bitsetComp );
};

#endif // GESTIONNAIREECS_H
