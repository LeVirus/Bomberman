#ifndef TILEMAPSYSTEM_H
#define TILEMAPSYSTEM_H

#include "system.hpp"
#include "constants.hpp"
#include <vector>

namespace ecs {
    struct PositionComponent;
}
struct TilemapBombermanComponent;

typedef std::vector<std::pair<TilemapBombermanComponent *,
            ecs::PositionComponent *>> VectPairCompTilemap;

class TilemapSystem: public ecs::System
{
private:
    VectPairCompTilemap mVectComponentTilemap;
public:
    TilemapSystem();
    virtual void execSystem();
    virtual void displaySystem()const;
    const VectPairCompTilemap &getVectCompTilemap()const;
};

#endif // TILEMAPSYSTEM_H
