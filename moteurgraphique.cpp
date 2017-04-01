#include "moteurgraphique.hpp"
#include "displaysystem.hpp"
#include "constants.hpp"
#include <cassert>
#include "moteur.hpp"



MoteurGraphique::MoteurGraphique()
{

	mVectTexture.push_back( sf::Texture() );
	assert( mVectTexture[0].loadFromFile( "../ProjetBomberman/Ressources/Texture/textBomberman.png" )
			&& "error while loading texture\n" );
}

const TileMap &MoteurGraphique::getTileMap()const
{
	return mTileMap;
}

void MoteurGraphique::initialiserFenetre()
{
	mFenetre.create( sf::VideoMode( 1040, 640 ), "Bomber", sf::Style::Default/*Fullscreen*/ );

	mCamera.reset( sf::FloatRect( 0, 0, WIDTH_SCREEN , HEIGHT_SCREEN ) );
	mFenetre.setView( mCamera );
	mFenetre.setFramerateLimit(60);

}

void MoteurGraphique::linkMainEngine( Moteur* ptrMoteur )
{
	assert( ptrMoteur && "ptrMoteur == null\n" );
	mPtrMoteurPrincipal = ptrMoteur;
}

void MoteurGraphique::loadTileMap(const Niveau &level, unsigned int uiNumEntity )
{
	mTileMap.configTileMap(level);
	mTileMap.loadLevel(level, uiNumEntity );
	mTileMap.adaptToScale( SIZE_SCALE, SIZE_SCALE );
}

unsigned int MoteurGraphique::loadSprite( unsigned int uiNumTexture, const sf::IntRect &positionSprite )
{
	mVectSprite.push_back( sf::Sprite() );
	unsigned int memNumSprite = mVectSprite.size() - 1;
	mVectSprite[ memNumSprite ].setTexture( mVectTexture[ uiNumTexture ] );
	mVectSprite[ memNumSprite ].setTextureRect( positionSprite );
	mVectSprite[ memNumSprite ].setScale( SIZE_SCALE, SIZE_SCALE );
	return memNumSprite;
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

		//affichage du tilemap
		if( uiNumSprite == SPRITE_TILEMAP )
		{
			mTileMap.setPosition(vector2DPos.mfX, vector2DPos.mfY);
			mFenetre.draw( mTileMap );
		}
		else//affichage d'un sprite contenu dans le tableau
		{
			mVectSprite[ uiNumSprite ] . setPosition( vector2DPos . mfX, vector2DPos . mfY );
			mFenetre.draw( mVectSprite[ uiNumSprite ] );
		}

	}
}

void MoteurGraphique::positionnerCaseTileMap( unsigned int uiNumEntity, unsigned int uiPositionX, unsigned int uiPositionY )
{
	ecs::PositionComponent * pc = mPtrMoteurPrincipal->getGestionnaireECS().getECSComponentManager() ->
			searchComponentByType< ecs::PositionComponent >( uiNumEntity, ecs::POSITION_COMPONENT );

	assert( pc && "pc == null\n");
	pc->vect2DPosComp.mfX = POSITION_LEVEL_X + uiPositionX * mPtrMoteurPrincipal->getNiveau().getLongueurTile();
	pc->vect2DPosComp.mfY = POSITION_LEVEL_Y + uiPositionY * mPtrMoteurPrincipal->getNiveau().getLargeurTile();

	std::cout << "mPtrMoteurPrincipal->getNiveau().getLongueurTile()" << mPtrMoteurPrincipal->getNiveau().getLongueurTile() <<
				 "mPtrMoteurPrincipal->getNiveau().getLargeurTile()" <<mPtrMoteurPrincipal->getNiveau().getLargeurTile() << "\n";
}

void MoteurGraphique::raffraichirEcran()
{
	mFenetre.clear( sf::Color::Black );
	//mTileMap.setScale( SIZE_SCALE, SIZE_SCALE );
	//mTileMap.setPosition( 0.0f, 0.0f );
	displayECSSprite();

	mFenetre.display();

}
