#include "explosionbombermansystem.hpp"

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
    System::execSystem();
    std::vector< unsigned int >::iterator it = mVectNumEntity.begin();
    for(; it != mVectNumEntity.end() ; ++it)
    {

    }
}

void ExplosionBombermanSystem::displaySystem() const
{}

