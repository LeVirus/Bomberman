#include "gestionnaireecs.hpp"
#include "inputbombermansystem.hpp"

GestionnaireECS::GestionnaireECS()
{
	mCompMan = &mEngineECS.getComponentManager();
	mSysMan = &mEngineECS.getSystemManager();
}

void GestionnaireECS::initECS()
{
	mCompMan->addEmplacementsForExternComponent( 2 );//inputComp & moveableComp
	mSysMan->bAddSystem( ecs::DISPLAY_SYSTEM );
	mSysMan->bAddExternSystem(std::make_unique<InputBombermanSystem>());
}

ecs::SystemManager *GestionnaireECS::getECSSystemManager()
{
	return mSysMan;
}

ecs::ComponentManager *GestionnaireECS::getECSComponentManager()
{
	return mCompMan;
}

/**
 * @brief GestionnaireECS::addEntity Création d'une entité avec les composants à y inclure.
 * @param bitsetComp La table contenant les composants à inclure.
 */
unsigned int GestionnaireECS::addEntity( const std::vector< bool > &bitsetComp )
{
	unsigned int mem = mEngineECS.AddEntity();
	for( unsigned int i = 0; i < bitsetComp.size(); ++i )
	{
		if( bitsetComp[ i ] )//composant
		{
			mEngineECS.bAddComponentToEntity( mem, i );
		}
		//this->getECSComponentManager()->displayComponent();
	}
	mCompMan->updateComponentFromEntity();
	//mEngineECS.displayVectEntity();

	return mem;
}
