#include "explosionbombermansystem.hpp"
#include "constants.hpp"
#include "positioncomponent.hpp"
#include "moteurgraphique.hpp"
#include "niveau.hpp"
#include "tableau2d.hpp"


ExplosionBombermanSystem::ExplosionBombermanSystem()
{
        if(! bAddComponentToSystem(ecs::POSITION_COMPONENT))
        {
            std::cout << "Erreur BombBombermanSystem ajout POSITION_COMPONENT.\n";
        }
    //    if(! bAddComponentToSystem(ecs::DISPLAY_COMPONENT))
    //    {
    //        std::cout << "Erreur BombBombermanSystem ajout DISPLAY_COMPONENT.\n";
    //    }
        if(! bAddComponentToSystem(BOMBER_FLAG_COMPONENT))
        {
            std::cout << "Erreur BombBombermanSystem ajout BOMBER_FLAG_COMPONENT.\n";
        }
        if(! bAddComponentToSystem(BOMBER_TIMER_COMPONENT))
        {
            std::cout << "Erreur BombBombermanSystem ajout BOMBER_TIMER_COMPONENT.\n";
        }
}

void ExplosionBombermanSystem::execSystem()
{
//    System::execSystem();
//    std::vector< unsigned int >::iterator it = mVectNumEntity.begin();
//    for(; it != mVectNumEntity.end() ; ++it)
//    {

//    }
}

void ExplosionBombermanSystem::displaySystem() const
{}

void ExplosionBombermanSystem::makeBombExplode(const ecs::PositionComponent &posComp)
{
    unsigned int caseX, caseY;
    unsigned int minX, maxX, minY, maxY;
    MoteurGraphique::static_getPositionsCurrentCase(posComp, caseX, caseY);
    getLimitForExplosion(caseX, caseY, minX, maxX, minY, maxY);
}

bool ExplosionBombermanSystem::getLimitForExplosion(const unsigned int caseX, const unsigned int caseY,
                                                    unsigned int &minX, unsigned int &maxX,
                                                    unsigned int &minY, unsigned int &maxY)
{
    const Tableau2D &tabNiveau = Niveau::getTabNiveau();
    if(tabNiveau.getLargeur() >= caseY || tabNiveau.getLongueur() >= caseX)
    {
        return false;
    }
    for(unsigned int maxX = caseX + 1; i < tabNiveau.getLongueur();++i)
    {
        //if(tabNiveau.getValAt(maxX, caseY) != )
    }
    return true;
}

