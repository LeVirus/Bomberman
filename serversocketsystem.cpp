#include "serversocketsystem.hpp"

ServerSocketSystem::ServerSocketSystem()
{
    if(! bAddComponentToSystem(BOMB_CONFIG_BOMBER_COMPONENT))
    {
        std::cout << "Erreur ServerSocketSystem ajout BOMB_CONFIG_BOMBER_COMPONENT.\n";
    }
}

void ServerSocketSystem::execSystem()
{
    System::execSystem();
    std::vector<unsigned int>::iterator it = mVectNumEntity.begin();
    for(; it != mVectNumEntity.end() ; ++it)
    {
//        FlagBombermanComponent *flagComponent = stairwayToComponentManager().searchComponentByType <FlagBombermanComponent>
//                (*it, FLAG_BOMBER_COMPONENT);

//        assert(flagComponent && "BombBombermanSystem::execSystem flagComponent is null\n");
//        if(flagComponent->muiNumFlag != FLAG_BOMB)
//        {
//            continue;
//        }

//        TimerBombermanComponent* timerComp = stairwayToComponentManager() .
//                    searchComponentByType <TimerBombermanComponent> (*it, TIMER_BOMBER_COMPONENT);
//        assert(timerComp && "BombBombermanSystem::execSystem :: timerComp == NULL\n");

//        BombConfigBombermanComponent* bombConfComp = stairwayToComponentManager() .
//                    searchComponentByType <BombConfigBombermanComponent> (*it, BOMB_CONFIG_BOMBER_COMPONENT);
//        assert(bombConfComp && "bombConfComp == NULL\n");
//        if(timerComp->mBombClockB.getElapsedTime().asMilliseconds() > 400)
//        {
//            changeSpriteBomb(*it, bombConfComp->mPreviousSizeLittle);
//            timerComp->mBombClockB.restart();
//        }

//        if(! timerComp->mLaunched)
//        {
//            timerComp->mBombClock.restart();
//            timerComp->mLaunched = true;
//        }
//        else
//        {
//            unsigned int timeElapsed = timerComp->mBombClock.getElapsedTime().asMilliseconds();
//            if(timeElapsed >= bombConfComp->mTimeBeforeExplosion)
//            {

//                if(! m_ptrExplosionSystem)
//                {
//                    m_ptrExplosionSystem = mptrSystemManager->searchSystemByType<ExplosionBombermanSystem>(EXPLOSION_BOMBER_SYSTEM);
//                    assert(m_ptrExplosionSystem && "BombBombermanSystem::BombBombermanSystem explosionSystem is null\n");
//                }
//                m_ptrExplosionSystem->makeBombExplode(*it);
//                mptrSystemManager->getptrEngine()->bRmEntity(*it);
//            }
//        }
    }
}

void ServerSocketSystem::displaySystem() const
{

}
