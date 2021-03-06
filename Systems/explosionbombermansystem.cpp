#include "explosionbombermansystem.hpp"
#include "tilemapbombermancomponent.hpp"
#include "constants.hpp"
#include "bombconfigbombermancomponent.hpp"
#include "positioncomponent.hpp"
#include "flagcomponent.hpp"
#include "timerbombermancomponent.hpp"
#include "tilemapbombermancomponent.hpp"
#include "playerconfigbombermancomponent.hpp"
#include "collrectboxcomponent.hpp"
#include "moteurgraphique.hpp"
#include "niveau.hpp"
#include "tableau2d.hpp"
#include "engine.hpp"
#include "vector2D.hpp"
#include <cassert>

ExplosionBombermanSystem::ExplosionBombermanSystem()

{
        if(! bAddComponentToSystem(ecs::POSITION_COMPONENT))
        {
            std::cout << "Erreur BombBombermanSystem ajout POSITION_COMPONENT.\n";
        }
        if(! bAddComponentToSystem(FLAG_BOMBER_COMPONENT))
        {
            std::cout << "Erreur BombBombermanSystem ajout BOMBER_FLAG_COMPONENT.\n";
        }
        if(! bAddComponentToSystem(TIMER_BOMBER_COMPONENT))
        {
            std::cout << "Erreur BombBombermanSystem ajout BOMBER_TIMER_COMPONENT.\n";
        }
}

void ExplosionBombermanSystem::execSystem()
{
    TilemapBombermanComponent *levelTilemapComp = stairwayToComponentManager().searchComponentByType <TilemapBombermanComponent>(
                    Niveau::getNumEntityLevel(), TILEMAP_BOMBER_COMPONENT);
    assert(levelTilemapComp && "levelTilemapComp null");
    System::execSystem();
    std::vector< unsigned int >::iterator it = mVectNumEntity.begin();
    for(; it != mVectNumEntity.end() ; ++it)
    {
        FlagBombermanComponent *flagComponent = stairwayToComponentManager().searchComponentByType <FlagBombermanComponent>
                (*it, FLAG_BOMBER_COMPONENT);

        assert(flagComponent && "flagComponent is null\n");
        if(flagComponent->muiNumFlag != FLAG_EXPLOSION)
        {
            continue;
        }

        TimerBombermanComponent* timerComp = stairwayToComponentManager() .
                    searchComponentByType <TimerBombermanComponent> (*it, TIMER_BOMBER_COMPONENT);
        assert(timerComp && "timerComp == NULL\n");

        if(! timerComp->mLaunched)
        {
            timerComp->mBombClock.restart();
            timerComp->mBombClockB.restart();
            timerComp->mBombClockC.restart();
            timerComp->mLaunched = true;
        }
        else
        {
            int timeElapsed = timerComp->mBombClock.getElapsedTime().asMilliseconds();
            if(timeElapsed >= mTimeExplosion)
            {
                //remove explosion entity
                vectTupleTriUI::iterator itt = mVectTupleMemWallDestroy.begin();
                while(itt != mVectTupleMemWallDestroy.end())
                {
                    if(std::get<0>(*itt) == ((*it)))
                    {
                        //first entity explosion num, others position of wall to destroy
                        destructWall(std::get<1>(*itt), std::get<2>(*itt), *levelTilemapComp);
                        itt = mVectTupleMemWallDestroy.erase(itt);
                        if(itt == mVectTupleMemWallDestroy.end())break;
                    }
                    else
                    {
                        ++itt;
                    }
                }
                //remove entity
                mptrSystemManager->getptrEngine()->bRmEntity(*it);
            }
            treatAnimationWallDestruction(*levelTilemapComp, *timerComp);

            float tenthExplosionTime = mTimeExplosion / 10;
            TilemapBombermanComponent *tilemapComp = stairwayToComponentManager().searchComponentByType <TilemapBombermanComponent>(
                           *it, TILEMAP_BOMBER_COMPONENT);
            assert(tilemapComp && "levelTilemapComp null");

            if(timerComp->mBombClockB.getElapsedTime().asMilliseconds() >= tenthExplosionTime)
            {
                if(timerComp->mBombClock.getElapsedTime().asMilliseconds() >= mTimeExplosion / 2)
                {
                    tilemapComp->mTabTilemap.addToAllCase(7);//lower explosion
                }
                else
                {
                    tilemapComp->mTabTilemap.substractToAllCase(7);//higher explosion
                }
                timerComp->mBombClockB.restart();
            }
        }
    }
}

void ExplosionBombermanSystem::displaySystem() const
{}

void ExplosionBombermanSystem::makeBombExplode(unsigned int numEntityBomb)
{
    unsigned int caseX, caseY;
    ecs::PositionComponent *posComponent = stairwayToComponentManager().searchComponentByType <ecs::PositionComponent>
            (numEntityBomb, ecs::POSITION_COMPONENT);
    assert(posComponent && "BombBombermanSystem::lauchBomb posComponent is null\n");
    MoteurGraphique::static_getPositionsCurrentCase(*posComponent, caseX, caseY, false);

    BombConfigBombermanComponent *bombConfComponent = stairwayToComponentManager().searchComponentByType <BombConfigBombermanComponent>
            (numEntityBomb, BOMB_CONFIG_BOMBER_COMPONENT);
    assert(bombConfComponent && "BombBombermanSystem::lauchBomb posComponent is null\n");

    createExplosions(caseX, caseY,  bombConfComponent->mRadiusExplosion);
}

void ExplosionBombermanSystem::destructWall(unsigned int x, unsigned int y,
                                            TilemapBombermanComponent &tilemapComp)
{
    tilemapComp.mTabTilemap.setValAt(x, y, TILE_EMPTY);
    unsigned int numEntityWall = Niveau::static_getNumWallEntityOnPosition(x, y);
    mptrSystemManager->getptrEngine()->bRmEntity(numEntityWall);
}

void ExplosionBombermanSystem::treatAnimationWallDestruction(TilemapBombermanComponent &tilemapComp,
                                                             TimerBombermanComponent &timerComp)
{
    if(timerComp.mBombClockC.getElapsedTime().asMilliseconds() > mTimeExplosion / 5)
    {
        timerComp.mBombClockC.restart();
        vectTupleTriUI::iterator it = mVectTupleMemWallDestroy.begin();
        for(;it != mVectTupleMemWallDestroy.end(); ++it)
        {
            unsigned int memValue = tilemapComp.mTabTilemap.getValAt(std::get<1>(*it), std::get<2>(*it));
            if(memValue == TILE_DESTRUCTIBLE_WALL)
            {
                memValue = TILE_DESTRUCTION_WALL_A;
            }
            else
            {
                if(memValue != TILE_DESTRUCTION_WALL_F)
                    ++memValue;
            }
            tilemapComp.mTabTilemap.setValAt(std::get<1>(*it), std::get<2>(*it), memValue);
        }
    }
}

bool ExplosionBombermanSystem::createExplosions(unsigned int caseX, unsigned int caseY, unsigned int explosionRadius)
{
    unsigned int minX = caseX - 1, maxX = caseX + 1, minY = caseY - 1, maxY = caseY + 1;
    unsigned int vertSizeUp = 0, vertSizeDown = 0, horizSizeLeft = 0, horizSizeRight = 0;
    bool maxXOk = false, minXOk = false, maxYOk = false, minYOk = false;
    vectTupleTriUI vectMemWallToDestroy;
    unsigned int entityLevel = Niveau::getNumEntityLevel();
    TilemapBombermanComponent *levelTileComponent = stairwayToComponentManager().searchComponentByType <TilemapBombermanComponent>
            (entityLevel, TILEMAP_BOMBER_COMPONENT);
    assert(levelTileComponent && "tileComponent is null\n");

    const Tableau2D &tabNiveau = levelTileComponent->mTabTilemap;

    if(tabNiveau.getHauteur() <= caseY || tabNiveau.getHauteur() <= caseX)
    {
        return false;
    }
    for(unsigned int i = 0; i < explosionRadius; ++i)
    {
        unsigned char memValue;
        if(!maxXOk)
        {
            memValue = tabNiveau.getValAt(maxX, caseY);
            if(memValue != TILE_EMPTY)
            {
                if(memValue == TILE_DESTRUCTIBLE_WALL)
                {
                    //0 for horizontal
                    vectMemWallToDestroy.push_back(tupleTriUi(0, maxX, caseY));
                }
                --maxX;
                maxXOk = true;
            }
            else
            {
                ++horizSizeRight;
                ++maxX;
            }
        }
        if(!minXOk)
        {
            memValue = tabNiveau.getValAt(minX, caseY);
            if(memValue != TILE_EMPTY)
            {
                if(memValue == TILE_DESTRUCTIBLE_WALL)
                {
                    vectMemWallToDestroy.push_back(tupleTriUi(0, minX, caseY));
                }

                ++minX;
                minXOk = true;
            }
            else
            {
                ++horizSizeLeft;
                --minX;
            }
        }
        if(!maxYOk)
        {
            memValue = tabNiveau.getValAt(caseX, maxY);
            if(memValue != TILE_EMPTY)
            {
                if(memValue == TILE_DESTRUCTIBLE_WALL)
                {
                    //1 for vertical
                    vectMemWallToDestroy.push_back(tupleTriUi(1, caseX, maxY));
                }
                --maxY;
                maxYOk = true;
            }
            else
            {
                ++vertSizeDown;
                ++maxY;
            }
        }
        if(!minYOk)
        {
            memValue = tabNiveau.getValAt(caseX, minY);
            if(tabNiveau.getValAt(caseX, minY) != TILE_EMPTY)
            {
                if(memValue == TILE_DESTRUCTIBLE_WALL)
                {
                    vectMemWallToDestroy.push_back(tupleTriUi(1, caseX, minY));
                }

                ++minY;
                minYOk = true;
            }
            else
            {
                ++vertSizeUp;
                --minY;
            }
        }
    }
    //fix offset in case of full explosion
    if(! maxXOk)--maxX;
    if(! minXOk)++minX;
    if(! maxYOk)--maxY;
    if(! minYOk)++minY;

    ////////////////////////////////////////////////////////
    //Create Explosions component

    //create horizontal explosion
    unsigned int horizExplosionEntity = createEntityExplosion(minX, caseY, explosionRadius, horizSizeLeft, horizSizeRight, false);
    //create vertical explosion
    unsigned int vertExplosionEntity = createEntityExplosion(caseX, minY, explosionRadius, vertSizeUp, vertSizeDown, true);
    memorizeWallToDestroy(vectMemWallToDestroy, vertExplosionEntity, horizExplosionEntity);
    return true;
}

unsigned int ExplosionBombermanSystem::createEntityExplosion(unsigned int positionCaseX, unsigned int positionCaseY, unsigned int explosionRadius,
                                                     unsigned int firstSize, unsigned int secondSize, bool vertical)
{
    unsigned int explosionEntity = createExplosionEntity();

    FlagBombermanComponent *flagComponent = stairwayToComponentManager().searchComponentByType <FlagBombermanComponent>
            (explosionEntity, FLAG_BOMBER_COMPONENT);
    assert(flagComponent && "flagComponent is null\n");
    flagComponent->muiNumFlag = FLAG_EXPLOSION;

    TilemapBombermanComponent *tileComponent = stairwayToComponentManager().searchComponentByType <TilemapBombermanComponent>
            (explosionEntity, TILEMAP_BOMBER_COMPONENT);
    assert(tileComponent && "tileComponent is null\n");

    tileComponent->mHeightTile = 16;
    tileComponent->mLenghtTile = 16;
    tileComponent->mNumAssociateTexture = TEXTURE_EXPLOSION;

    unsigned int totalSize = firstSize + 1 + secondSize;

    if(vertical)
        tileComponent->mTabTilemap.resize(1, totalSize);
    else
        tileComponent->mTabTilemap.resize(totalSize, 1);

    for(unsigned int i = 0; i < totalSize; ++i)
    {
        //check if the radius explosion is the longuest it could be in the first part
        if(i == 0 && firstSize == explosionRadius)
        {
            //7 * 4 = 28 for the fourth picture associate to explosion tile
            if(vertical)
                tileComponent->mTabTilemap.setValAt(0, i, EXPLOSION_END_UP + 28);
            else
                tileComponent->mTabTilemap.setValAt(i, 0, EXPLOSION_END_LEFT + 28);
        }
        //check the middle of explosion
        else if(i == firstSize)
        {
            if(vertical)
                tileComponent->mTabTilemap.setValAt(0, i, EXPLOSION_CENTER + 28);
            else
                tileComponent->mTabTilemap.setValAt(i, 0, EXPLOSION_CENTER + 28);
        }
        //check if the radius explosion is the longuest it could be in the second part
        else if(i == totalSize - 1 && secondSize == explosionRadius)
        {
            if(vertical)
                tileComponent->mTabTilemap.setValAt(0, i, EXPLOSION_END_DOWN + 28);
            else
                tileComponent->mTabTilemap.setValAt(i, 0, EXPLOSION_END_RIGHT + 28);
        }
        else
        {
            if(vertical)
                tileComponent->mTabTilemap.setValAt(0, i, EXPLOSION_VERTICAL_MIDDLE + 28);
            else
                tileComponent->mTabTilemap.setValAt(i, 0, EXPLOSION_HORIZONTAL_MIDDLE + 28);
        }
    }
    loadTilePosition(*tileComponent);
    //position the tilemap
    ecs::PositionComponent *posComponent = stairwayToComponentManager().searchComponentByType <ecs::PositionComponent>
            (explosionEntity, ecs::POSITION_COMPONENT);
    assert(posComponent && "posComponent is null\n");
    MoteurGraphique::static_positionnerCaseTileMap(*posComponent, positionCaseX, positionCaseY);

    ecs::CollRectBoxComponent *collComponent = stairwayToComponentManager().searchComponentByType <ecs::CollRectBoxComponent>
            (explosionEntity, ecs::COLL_RECTBOX_COMPONENT);
    assert(collComponent && "collComponent is null\n");
    if(vertical)
    {
        collComponent->mRectBox.mSetHeightRectBox(totalSize * MoteurGraphique::getCaseHeight());
        collComponent->mRectBox.mSetLenghtRectBox(MoteurGraphique::getCaseLenght());
    }
    else
    {
        collComponent->mRectBox.mSetHeightRectBox(MoteurGraphique::getCaseLenght());
        collComponent->mRectBox.mSetLenghtRectBox(totalSize * MoteurGraphique::getCaseHeight());
    }
    //positionner le décallage
    collComponent->mVect2dVectOrigins.mfX = 0;
    collComponent->mVect2dVectOrigins.mfY = 0;
    collComponent->mRectBox.mSetOriginsRectBox(posComponent->vect2DPosComp);

    return explosionEntity;
}

void ExplosionBombermanSystem::loadTilePosition(TilemapBombermanComponent &tileComp)
{
    tileComp.mvectPositionTile.push_back({119, 85});//0 end left
    tileComp.mvectPositionTile.push_back({34, 85});//1 end right
    tileComp.mvectPositionTile.push_back({85, 85});//2 EXPLOSION_CENTER
    tileComp.mvectPositionTile.push_back({51, 85});//3 EXPLOSION_VERTICAL_MIDDLE
    tileComp.mvectPositionTile.push_back({0, 85});// 4 EXPLOSION_END_UP
    tileComp.mvectPositionTile.push_back({136, 17});//5 EXPLOSION_END_DOWN
    tileComp.mvectPositionTile.push_back({68, 85});//6 EXPLOSION_HORIZONTAL_MIDDLE

    tileComp.mvectPositionTile.push_back({119, 68});//0 end left
    tileComp.mvectPositionTile.push_back({34, 68});//1 end right
    tileComp.mvectPositionTile.push_back({85, 68});//2 EXPLOSION_CENTER
    tileComp.mvectPositionTile.push_back({51, 68});//3 EXPLOSION_VERTICAL_MIDDLE
    tileComp.mvectPositionTile.push_back({0, 68});// 4 EXPLOSION_END_UP
    tileComp.mvectPositionTile.push_back({136, 34});//5 EXPLOSION_END_DOWN
    tileComp.mvectPositionTile.push_back({68, 68});//6 EXPLOSION_HORIZONTAL_MIDDLE

    tileComp.mvectPositionTile.push_back({119, 51});//0 end left
    tileComp.mvectPositionTile.push_back({34, 51});//1 end right
    tileComp.mvectPositionTile.push_back({85, 51});//2 EXPLOSION_CENTER
    tileComp.mvectPositionTile.push_back({51, 51});//3 EXPLOSION_VERTICAL_MIDDLE
    tileComp.mvectPositionTile.push_back({0, 51});// 4 EXPLOSION_END_UP
    tileComp.mvectPositionTile.push_back({136, 51});//5 EXPLOSION_END_DOWN
    tileComp.mvectPositionTile.push_back({68, 51});//6 EXPLOSION_HORIZONTAL_MIDDLE

    tileComp.mvectPositionTile.push_back({119, 34});//0 end left
    tileComp.mvectPositionTile.push_back({34, 34});//1 end right
    tileComp.mvectPositionTile.push_back({85, 34});//2 EXPLOSION_CENTER
    tileComp.mvectPositionTile.push_back({51, 34});//3 EXPLOSION_VERTICAL_MIDDLE
    tileComp.mvectPositionTile.push_back({0, 34});// 4 EXPLOSION_END_UP
    tileComp.mvectPositionTile.push_back({136, 68});//5 EXPLOSION_END_DOWN
    tileComp.mvectPositionTile.push_back({68, 34});//6 EXPLOSION_HORIZONTAL_MIDDLE

    tileComp.mvectPositionTile.push_back({119, 17});//0 end left
    tileComp.mvectPositionTile.push_back({34, 17});//1 end right
    tileComp.mvectPositionTile.push_back({85, 17});//2 EXPLOSION_CENTER
    tileComp.mvectPositionTile.push_back({51, 17});//3 EXPLOSION_VERTICAL_MIDDLE
    tileComp.mvectPositionTile.push_back({0, 17});// 4 EXPLOSION_END_UP
    tileComp.mvectPositionTile.push_back({136, 85});//5 EXPLOSION_END_DOWN
    tileComp.mvectPositionTile.push_back({68, 17});//6 EXPLOSION_HORIZONTAL_MIDDLE
}

void ExplosionBombermanSystem::memorizeWallToDestroy(const vectTupleTriUI &vectWallToDestroy,
                                                     unsigned int vertEntity, unsigned int horizEntity)
{
    vectTupleTriUI::const_iterator it = vectWallToDestroy.begin();
    for(; it != vectWallToDestroy.end();++it)
    {
        if(std::get<0>(*it) == 0)
        {
            mVectTupleMemWallDestroy.push_back(tupleTriUi(horizEntity, std::get<1>(*it), std::get<2>(*it)));
        }
        else if(std::get<0>(*it) == 1)
        {
            mVectTupleMemWallDestroy.push_back(tupleTriUi(vertEntity, std::get<1>(*it), std::get<2>(*it)));
        }
    }
}

unsigned int ExplosionBombermanSystem::createExplosionEntity()
{
    //create entity
    unsigned int numCreatedEntity = mptrSystemManager->getptrEngine()->AddEntity();
    ecs::Engine *ECSEngine = mptrSystemManager->getptrEngine();
    ECSEngine->bAddComponentToEntity( numCreatedEntity, ecs::POSITION_COMPONENT );
    ECSEngine->bAddComponentToEntity( numCreatedEntity, ecs::COLL_RECTBOX_COMPONENT );
    ECSEngine->bAddComponentToEntity( numCreatedEntity, FLAG_BOMBER_COMPONENT );
    ECSEngine->bAddComponentToEntity( numCreatedEntity, TIMER_BOMBER_COMPONENT );
    ECSEngine->bAddComponentToEntity( numCreatedEntity, TILEMAP_BOMBER_COMPONENT );
    stairwayToComponentManager().updateComponentFromEntity();
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<FlagBombermanComponent>());
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<TimerBombermanComponent>());
    stairwayToComponentManager().instanciateExternComponent(numCreatedEntity, std::make_unique<TilemapBombermanComponent>());
    return numCreatedEntity;
}
