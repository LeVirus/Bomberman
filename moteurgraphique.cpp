#include "moteurgraphique.hpp"
#include "displaysystem.hpp"
#include "tilemapsystem.hpp"
#include "constants.hpp"
#include <cassert>
#include <iostream>
#include "moteur.hpp"
#include "jeu.hpp"
#include "positioncomponent.hpp"
#include <SFML/Graphics.hpp>


float MoteurGraphique::mCaseLenght;
float MoteurGraphique::mCaseHeight;
std::vector<sf::Texture> MoteurGraphique::mVectTexture;

MoteurGraphique::MoteurGraphique()
{
    mVectTexture.resize(3);
    mVectTexture[0].loadFromFile("../Bomberman/Ressources/Texture/textBomberman.png");
    mVectTexture[1].loadFromFile("../Bomberman/Ressources/Texture/textExplode.png");
    mVectTexture[2].loadFromFile("../Bomberman/Ressources/Texture/textLevelTileMap.png");
}

void MoteurGraphique::loadBaseSprites()
{
    loadBombermanSprite();
    loadBomBSprite();
}

void MoteurGraphique::getEventFromWindows(sf::Event &event)
{
    mFenetre.pollEvent(event);
}

void MoteurGraphique::loadLevelTileMap(Niveau &niv, uint32_t numNiv)
{
    uint32_t memEntity = mPtrMoteurPrincipal->initLevel();

    TilemapBombermanComponent *tmc = mPtrMoteurPrincipal->getGestionnaireECS().getECSComponentManager() ->
            searchComponentByType<TilemapBombermanComponent>(memEntity, TILEMAP_BOMBER_COMPONENT);
    assert(tmc && "Moteur::loadTileMap TilemapBombermanComponent == null\n");
    niv.loadLevel(numNiv, memEntity, *tmc);
    niv.adaptToScale(SIZE_SCALE, SIZE_SCALE);
    //récupération et modification des composants
    memorizeSizeTile(niv.getLongueurTile(), niv.getLargeurTile());
}

void MoteurGraphique::loadLevelTileMapFromServer(Niveau &niv, const NetworkLevelData &dataLevel)
{
    uint32_t memEntity = mPtrMoteurPrincipal->initLevel();
    TilemapBombermanComponent *tmc = mPtrMoteurPrincipal->getGestionnaireECS().getECSComponentManager() ->
            searchComponentByType<TilemapBombermanComponent>(memEntity, TILEMAP_BOMBER_COMPONENT);
    assert(tmc && "Moteur::loadTileMap TilemapBombermanComponent == null");
    bool res = niv.loadLevelFromServer(memEntity, *tmc, dataLevel);
    assert(res && "loadLevelFromServer fail");
    niv.adaptToScale(SIZE_SCALE, SIZE_SCALE);
    //récupération et modification des composants
    memorizeSizeTile(niv.getLongueurTile(), niv.getLargeurTile());
}

const std::vector<TileMap> &MoteurGraphique::getTileMap()const
{
    return mVectTileMap;
}

void MoteurGraphique::loadBombermanSprite()
{
    uint32_t cursorX = 3, cursorY = 19;
    for(uint32_t i = 0 ; i < 18; ++i)
    {
        if(i == 3 || i == 12)
        {
            cursorY += 26;
            cursorX = 3;
        }
        loadSprite(TEXTURE_BOMBERMAN, sf::IntRect(cursorX, cursorY, 16, 25));
        cursorX += 17;
    }
}

void MoteurGraphique::loadBomBSprite()
{
    uint32_t cursorX = 0, cursorY = 0;
    for(uint32_t i = 0 ; i < 3; ++i)
    {
        loadSprite(TEXTURE_EXPLOSION, sf::IntRect(cursorX, cursorY, 16, 16));
        cursorX += 17;
    }
}

void MoteurGraphique::initialiserFenetre()
{
    mFenetre.create( sf::VideoMode( 550, 590 ), "Bomber", sf::Style::/*Fullscreen*/Default);
    mCamera.reset( sf::FloatRect( 130, 50, 1024/*WIDTH_SCREEN*/ , 930/*HEIGHT_SCREEN*/ ) );
	mFenetre.setView( mCamera );
	mFenetre.setFramerateLimit(60);
}

void MoteurGraphique::linkMainEngine( Moteur* ptrMoteur )
{
	assert( ptrMoteur && "ptrMoteur == null\n" );
	mPtrMoteurPrincipal = ptrMoteur;
}

void MoteurGraphique::memorizeSizeTile(unsigned int lenghtTile, unsigned int heightTile)
{
    mCaseLenght = lenghtTile;
    mCaseHeight = heightTile;
}

unsigned int MoteurGraphique::loadSprite( unsigned int uiNumTexture, const sf::IntRect &positionSprite )
{
	mVectSprite.push_back( sf::Sprite() );
	unsigned int memNumSprite = mVectSprite.size() - 1;
    mVectSprite[memNumSprite].setTexture(mVectTexture[uiNumTexture]);
    mVectSprite[memNumSprite].setTextureRect(positionSprite);
    mVectSprite[memNumSprite].setScale(SIZE_SCALE, SIZE_SCALE);
	return memNumSprite;
}

//Waring some tilemap have to be display BEFORE sprites and other AFTER
void MoteurGraphique::displayECSTilemap()
{
    const VectPairCompTilemap &vectCompTilemap = mPtrMoteurPrincipal->
            getGestionnaireECS().getECSSystemManager()->
            searchSystemByType<TilemapSystem> (TILEMAP_BOMBER_SYSTEM)->
            getVectCompTilemap();
    mVectTileMap.clear();
    VectPairCompTilemap::const_iterator it = vectCompTilemap.begin();
    for(unsigned int i = 0; it != vectCompTilemap.end(); ++it, ++i)
    {
        mVectTileMap.push_back(TileMap());
        mVectTileMap[i].setPosition((*it).second->vect2DPosComp.mfX, (*it).second->vect2DPosComp.mfY);
        mVectTileMap[i].setScale(SIZE_SCALE, SIZE_SCALE);
        mVectTileMap[i].configureTileMap(*(*it).first);
        mFenetre.draw(mVectTileMap[i]);
    }
}

void MoteurGraphique::displayECSSprite()
{
    mVectComponentDisplaySystem = mPtrMoteurPrincipal -> getGestionnaireECS().getECSSystemManager() ->
            searchSystemByType< ecs::DisplaySystem > ( ecs::DISPLAY_SYSTEM ) ->
            getVectComponentDisplaySystem();
    assert( mVectComponentDisplaySystem && "mVectComponentDisplaySystem == null\n" );
    for( unsigned int i = 0; i < mVectComponentDisplaySystem -> size() ; ++i )
    {
        if(! (* mVectComponentDisplaySystem )[ i ].first->mVisible)continue;
        unsigned int uiNumSprite = (* mVectComponentDisplaySystem )[ i ].first -> muiNumSprite;
        const ecs::Vector2D vector2DPos = (* mVectComponentDisplaySystem )[ i ].second -> vect2DPosComp;
        assert((uiNumSprite < mVectSprite.size()) && "mVectSprite overflow\n");

        mVectSprite[uiNumSprite].setPosition(vector2DPos.mfX, vector2DPos.mfY);
        mFenetre.draw(mVectSprite[uiNumSprite]);
    }
}

const std::vector<sf::Texture> &MoteurGraphique::static_getTabTexture()
{
    return mVectTexture;
}

void MoteurGraphique::static_positionnerCaseTileMap(ecs::PositionComponent &posComp,
                                                    unsigned int uiPositionX,
                                                    unsigned int uiPositionY, bool bomberman)
{
    posComp.vect2DPosComp.mfX = POSITION_LEVEL_X + uiPositionX * mCaseLenght;
    posComp.vect2DPosComp.mfY = POSITION_LEVEL_Y + uiPositionY * mCaseHeight;
    if(bomberman)
    {
        posComp.vect2DPosComp.mfY -= mCaseHeight / 1.6f ;
    }
}

//a améliorer
void MoteurGraphique::static_positionComponentCenterCurrentCase(ecs::PositionComponent &positionComp)
{
//    float thirdCaseLenght = mCaseLenght / 3 ;
//    float thirdCaseHeight = mCaseHeight / 3 ;
    unsigned int caseX;
    unsigned int caseY;
    static_getPositionsCurrentCase(positionComp, caseX, caseY);
    positionComp.vect2DPosComp.mfX = POSITION_LEVEL_X + caseX * mCaseLenght/* + thirdCaseLenght*/;
    positionComp.vect2DPosComp.mfY = POSITION_LEVEL_Y + caseY * mCaseHeight /*+ thirdCaseHeight*/;
}

void MoteurGraphique::static_getPositionsCurrentCase(const ecs::PositionComponent &positionComp,
                                                     unsigned int &caseX, unsigned int &caseY, bool launchBomb)
{
    float fCaseX = (positionComp.vect2DPosComp.mfX - POSITION_LEVEL_X) / mCaseLenght;
    float fCaseY = (positionComp.vect2DPosComp.mfY - POSITION_LEVEL_Y) / mCaseHeight;
    caseX = fCaseX;
    caseY = fCaseY;
    if(! launchBomb)return;

    fCaseY -= caseY;
    fCaseX -= caseX;
    if(fCaseX > 0.30)
    {
        ++caseX;
    }

    if(fCaseY > 0.20)
    {
        ++caseY;
    }
}

void MoteurGraphique::raffraichirEcran()
{
    mFenetre.clear(sf::Color::Black);
    displayECSTilemap();
	displayECSSprite();
    mFenetre.display();
}
