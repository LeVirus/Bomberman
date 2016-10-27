#include "gestionnaireecs.hpp"

GestionnaireECS::GestionnaireECS()
{
	mCompMan = &mEngineECS.getComponentManager();
	mSysMan = &mEngineECS.getSystemManager();
}
