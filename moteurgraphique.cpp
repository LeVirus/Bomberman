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

MoteurGraphique::MoteurGraphique()
{

	mVectTexture.push_back( sf::Texture() );
    assert( mVectTexture[0].loadFromFile( "../Bomberman/Ressources/Texture/textBomberman.png" )
            && "error while loading texture\n" );
}

void MoteurGraphique::getEventFromWindows(sf::Event &event)
{
    mFenetre.pollEvent(event);
}

const std::vector<TileMap> &MoteurGraphique::getTileMap()const
{
    return mVectTileMap;
}

void MoteurGraphique::initialiserFenetre()
{
    mFenetre.create( sf::VideoMode( 1040, 640 ), "Bomber", sf::Style::/*Fullscreen*/Default );

	mCamera.reset( sf::FloatRect( 0, 0, WIDTH_SCREEN , HEIGHT_SCREEN ) );
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
//    //mVectTileMap.configTileMap(level);
//    //mVectTileMap.loadLevel(level, uiNumEntity);
//    //mVectTileMap.adaptToScale(SIZE_SCALE, SIZE_SCALE);

    mCaseLenght = lenghtTile;
    mCaseHeight = heightTile;
}

unsigned int MoteurGraphique::loadSprite( unsigned int uiNumTexture, const sf::IntRect &positionSprite )
{
	mVectSprite.push_back( sf::Sprite() );
	unsigned int memNumSprite = mVectSprite.size() - 1;
    mVectSprite[memNumSprite].setTexture(mVectTexture[ uiNumTexture]);
    mVectSprite[memNumSprite].setTextureRect( positionSprite);
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
//        mVectTileMap[i].setPosition(0, 0);
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
        unsigned int uiNumSprite = (* mVectComponentDisplaySystem )[ i ].first -> muiNumSprite;
        const ecs::Vector2D vector2DPos = (* mVectComponentDisplaySystem )[ i ].second -> vect2DPosComp;
        assert( ( ( uiNumSprite == SPRITE_TILEMAP ) || ( uiNumSprite < mVectSprite.size() ) )
                && "mVectSprite overflow\n" );

        mVectSprite[ uiNumSprite ] . setPosition( vector2DPos . mfX, vector2DPos . mfY );
        mFenetre.draw( mVectSprite[ uiNumSprite ] );
    }
}

void MoteurGraphique::positionnerCaseTileMap(unsigned int uiNumEntity, unsigned int uiPositionX, unsigned int uiPositionY )
{
	ecs::PositionComponent * pc = mPtrMoteurPrincipal->getGestionnaireECS().getECSComponentManager() ->
			searchComponentByType< ecs::PositionComponent >( uiNumEntity, ecs::POSITION_COMPONENT );

	assert( pc && "pc == null\n");
    pc->vect2DPosComp.mfX = POSITION_LEVEL_X + uiPositionX * mCaseLenght;
    pc->vect2DPosComp.mfY = POSITION_LEVEL_Y + uiPositionY * mCaseHeight;

}

//a améliorer
void MoteurGraphique::static_positionComponentCenterCurrentCase(ecs::PositionComponent &positionComp)
{
    float thirdCaseLenght = mCaseLenght / 3 ;
    float thirdCaseHeight = mCaseHeight / 3 ;

    unsigned int caseX;
    unsigned int caseY;

    static_getPositionsCurrentCase(positionComp, caseX, caseY);

    positionComp.vect2DPosComp.mfX = POSITION_LEVEL_X + caseX * mCaseLenght + thirdCaseLenght;
    positionComp.vect2DPosComp.mfY = POSITION_LEVEL_Y + caseY * mCaseHeight + thirdCaseHeight;
}

void MoteurGraphique::static_getPositionsCurrentCase(const ecs::PositionComponent &positionComp,
                                                     unsigned int &caseX, unsigned int &caseY)
{
    float thirdCaseLenght = mCaseLenght / 3;
    float thirdCaseHeight = mCaseHeight / 3;
    caseX = (unsigned int)(positionComp.vect2DPosComp.mfX - POSITION_LEVEL_X) / mCaseLenght;
    caseY = (unsigned int)(positionComp.vect2DPosComp.mfY - POSITION_LEVEL_Y) / mCaseHeight;

    if((unsigned int)(positionComp.vect2DPosComp.mfX - POSITION_LEVEL_X) % (unsigned int)mCaseLenght > (unsigned int)thirdCaseLenght)
    {
        ++caseX;
    }

    if((unsigned int)(positionComp.vect2DPosComp.mfY - POSITION_LEVEL_Y) % (unsigned int)mCaseHeight > (unsigned int)thirdCaseHeight)
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
