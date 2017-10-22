#include "gestionnaireecs.hpp"
#include "inputbombermansystem.hpp"
#include "collisionbombermansystem.hpp"
#include "bombbombermansystem.hpp"

GestionnaireECS::GestionnaireECS()
{
	mCompMan = &mEngineECS.getComponentManager();
	mSysMan = &mEngineECS.getSystemManager();
}

void GestionnaireECS::initECS()
{
    mCompMan->addEmplacementsForExternComponent( 4 );//inputComp, moveableComp, timerComp & flagComp
	mSysMan->bAddSystem( ecs::DISPLAY_SYSTEM );
	mSysMan->bAddExternSystem(std::make_unique<InputBombermanSystem>());
	mSysMan->bAddExternSystem(std::make_unique<CollisionBombermanSystem>());
    mSysMan->bAddExternSystem(std::make_unique<BombBombermanSystem>());
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
