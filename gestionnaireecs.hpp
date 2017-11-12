#ifndef GESTIONNAIREECS_H
#define GESTIONNAIREECS_H

#include "engine.hpp"
#include <bitset>

class Moteur;

class GestionnaireECS
{
private:
	ecs::Engine mEngineECS;
	ecs::ComponentManager* mCompMan = nullptr;
	ecs::SystemManager* mSysMan = nullptr;
    const Moteur &mRefMoteur;
public:
    GestionnaireECS(Moteur &refMoteur);
	ecs::SystemManager *getECSSystemManager();
	ecs::ComponentManager *getECSComponentManager();
    ecs::Engine *getECSEngine();
    void initECS();
	unsigned int addEntity( const std::vector< bool > &bitsetComp );
    const Moteur &getMoteur()const;
};

#endif // GESTIONNAIREECS_H
