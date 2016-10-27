#ifndef GESTIONNAIREECS_H
#define GESTIONNAIREECS_H

#include "engine.hpp"


class GestionnaireECS
{
private:
	Engine mEngineECS;
	ComponentManager* mCompMan = nullptr;
	SystemManager* mSysMan = nullptr;
public:
	GestionnaireECS();
};

#endif // GESTIONNAIREECS_H
