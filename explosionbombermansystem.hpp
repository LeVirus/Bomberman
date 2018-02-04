#ifndef EXPLOSIONBOMBERMANSYSTEM_H
#define EXPLOSIONBOMBERMANSYSTEM_H

namespace ecs
{
    struct PositionComponent;
}

struct TilemapBombermanComponent;


#include "system.hpp"
#include <tuple>
#include <vector>

//first entity explosion num, others position of wall to destroy
typedef std::tuple<unsigned int, unsigned int, unsigned int> tupleTriUi;
typedef std::vector<tupleTriUi> vectTupleTriUI;

class ExplosionBombermanSystem : public ecs::System
{
private:
    vectTupleTriUI mVectTupleMemWallDestroy;
    unsigned int mTimeExplosion = 1500;
    TilemapBombermanComponent *mLevelTilemapComp = nullptr;
    unsigned int createExplosionEntity();
    bool createExplosions(unsigned int caseX, unsigned int caseY, unsigned int explosionRadius);
    unsigned int createEntityExplosion(unsigned int positionCaseX, unsigned int positionCaseY, unsigned int explosionRadius,
                               unsigned int firstSize, unsigned int secondSize, bool vertical);
    void loadTilePosition(TilemapBombermanComponent &tileComp);
    void memorizeWallToDestroy(const vectTupleTriUI &vectWallToDestroy, unsigned int vertEntity, unsigned int horizEntity);
    void destructWall(unsigned int x, unsigned int y, TilemapBombermanComponent &tilemapComp);
public:
    ExplosionBombermanSystem();
    virtual void execSystem();
    virtual void displaySystem()const;
    void makeBombExplode(unsigned int numEntityBomb);
};

#endif // EXPLOSIONBOMBERMANSYSTEM_H
