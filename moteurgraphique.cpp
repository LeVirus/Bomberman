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

const std::pair<unsigned int, unsigned int> &MoteurGraphique::getPositionLevel() const
{
	return mPairPositionLevel;
}

void MoteurGraphique::initialiserFenetre()
{
	mFenetre.create( sf::VideoMode( WIDTH_SCREEN, HEIGHT_SCREEN ), "Bomber", sf::Style::Fullscreen );

	mCamera.reset( sf::FloatRect( 0, 0, WIDTH_SCREEN , HEIGHT_SCREEN ) );
	mFenetre.setView( mCamera );
	mFenetre.setFramerateLimit(60);

}

void MoteurGraphique::linkMainEngine( Moteur* ptrMoteur )
{
	assert( ptrMoteur && "ptrMoteur == null\n" );
	mPtrMoteurPrincipal = ptrMoteur;
}

bool MoteurGraphique::loadTileMap( const std::string &strPathConfFile , unsigned int uiNumEntity )
{
	mPairPositionLevel.first = mTileMap.getPosition().x;
	mPairPositionLevel.second = mTileMap.getPosition().y;
	mTileMap.loadLevel( strPathConfFile, uiNumEntity );
	positionnerTileMap( uiNumEntity );
	//mTileMap.displayTileMap();
}

unsigned int MoteurGraphique::loadSprite( unsigned int uiNumTexture, const sf::IntRect &positionSprite )
{

	mVectSprite.push_back( sf::Sprite() );
	unsigned int memNumSprite = mVectSprite.size() - 1;
	mVectSprite[ memNumSprite ] .setTexture( mVectTexture[ uiNumTexture ] );
	mVectSprite[ memNumSprite ] .setTextureRect( positionSprite );
	return memNumSprite;
}

void MoteurGraphique::displayECSSprite()
{
	mVectComponentDisplaySystem = mPtrMoteurPrincipal -> getGestionnaireECS().getECSSystemManager()->
			searchSystemByType< ecs::DisplaySystem > ( ecs::DISPLAY_SYSTEM ) ->
			getVectComponentDisplaySystem();
	assert( mVectComponentDisplaySystem && "mVectComponentDisplaySystem == null\n" );
	for( unsigned int i = 0; i < mVectComponentDisplaySystem -> size() ; ++i )
	{
		unsigned int uiNumSprite = (* mVectComponentDisplaySystem )[ i ].first -> muiNumSprite;
		const ecs::Vector2D vector2DPos = (* mVectComponentDisplaySystem )[ i ].second -> vect2DPosComp;
		assert( uiNumSprite == SPRITE_TILEMAP || uiNumSprite < mVectSprite.size() && "mVectSprite overflow\n" );

		//affichage du tilemap
		if( uiNumSprite == SPRITE_TILEMAP )
		{
			mTileMap.setPosition( vector2DPos . mfX, vector2DPos . mfY );
			mFenetre.draw( mTileMap );
		}
		else//affichage d'un sprite contenu dans le tableau
		{
			mVectSprite[ uiNumSprite ] . setPosition( vector2DPos . mfX, vector2DPos . mfY );
			mFenetre.draw( mVectSprite[ uiNumSprite ] );
		}

	}
}

void MoteurGraphique::positionnerTileMap( unsigned int uiPosition )
{

}

void MoteurGraphique::raffraichirEcran()
{
	mFenetre.clear( sf::Color::Black );
	mTileMap.setScale( 3.0f, 3.0f );
	mTileMap.setPosition( 0.0f, 0.0f );
	displayECSSprite();

	mFenetre.display();

}
