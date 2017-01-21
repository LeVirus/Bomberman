#include "gestionnaireecs.hpp"

GestionnaireECS::GestionnaireECS()
{
	mCompMan = &mEngineECS.getComponentManager();
	mSysMan = &mEngineECS.getSystemManager();
}

void GestionnaireECS::initECS()
{
	mSysMan->bAddSystem( ecs::DISPLAY_SYSTEM );
}

ecs::SystemManager *GestionnaireECS::getECSSystemManager()
{
	return mSysMan;
}

/**
 * @brief GestionnaireECS::addEntity Création d'une entité avec les composants à y inclure.
 * @param bitsetComp La table contenant les composants à inclure.
 */
unsigned int GestionnaireECS::addEntity( const std::bitset< ecs::NUMBR_COMPONENT > &bitsetComp )
{
	unsigned int mem = mEngineECS.AddEntity();
	for( unsigned int i = 0; i < bitsetComp.size(); ++i )
	{
		if( bitsetComp[ i ] )//composant
		{
			mEngineECS.bAddComponentToEntity( mem, i );
		}
	}
	return mem;
}
