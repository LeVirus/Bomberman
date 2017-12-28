#include "gestionnaireecs.hpp"
#include "inputbombermansystem.hpp"
#include "collisionbombermansystem.hpp"
#include "bombbombermansystem.hpp"
#include "explosionbombermansystem.hpp"
#include "moteur.hpp"

GestionnaireECS::GestionnaireECS(Moteur &refMoteur):mRefMoteur(refMoteur)
{
	mCompMan = &mEngineECS.getComponentManager();
	mSysMan = &mEngineECS.getSystemManager();
}

void GestionnaireECS::initECS()
{
    mCompMan->addEmplacementsForExternComponent(7);
    mSysMan->bAddSystem(ecs::DISPLAY_SYSTEM);
	mSysMan->bAddExternSystem(std::make_unique<InputBombermanSystem>());
	mSysMan->bAddExternSystem(std::make_unique<CollisionBombermanSystem>());
    mSysMan->bAddExternSystem(std::make_unique<ExplosionBombermanSystem>());
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

ecs::Engine *GestionnaireECS::getECSEngine()
{
    return &mEngineECS;
}

/**
 * @brief GestionnaireECS::addEntity Création d'une entité avec les composants à y inclure.
 * @param bitsetComp La table contenant les composants à inclure.
 */
unsigned int GestionnaireECS::addEntity(const std::vector< bool > &bitsetComp)
{
	unsigned int mem = mEngineECS.AddEntity();
    for(unsigned int i = 0; i < bitsetComp.size(); ++i)
	{
        if(bitsetComp[i])//composant
		{
            mEngineECS.bAddComponentToEntity(mem, i);
		}
	}
	mCompMan->updateComponentFromEntity();
    return mem;
}

const Moteur &GestionnaireECS::getMoteur() const
{
    return mRefMoteur;
}
