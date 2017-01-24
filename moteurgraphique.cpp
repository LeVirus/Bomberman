#include "moteurgraphique.hpp"
#include "displaysystem.hpp"
#include "constants.hpp"
#include <cassert>
#include "moteur.hpp"



void MoteurGraphique::initialiserFenetre()
{
	mFenetre.create(sf::VideoMode( WIDTH_SCREEN, HEIGHT_SCREEN ), "Bomber", sf::Style::Fullscreen );

	mCamera.reset( sf::FloatRect( 0, 0, 800 , 600 ) );
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
	mTileMap.loadLevel( strPathConfFile, uiNumEntity );
	positionnerTileMap( uiNumEntity );
	//mTileMap.displayTileMap();
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
			mVectSprite[ uiNumSprite ] -> setPosition( vector2DPos . mfX, vector2DPos . mfY );
			mFenetre.draw( *mVectSprite[ uiNumSprite ].get() );
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

	mFenetre.draw( mTileMap );//a modifier pour l'intégrer a l'ECS
	mFenetre.display();

}
