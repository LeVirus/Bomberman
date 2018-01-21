#include "tableau2d.hpp"
#include <iostream>


/**
 * @brief Constructeur de la classe Tableau2D
 * Définition de la longueur et de la largeur du tableau
 * @param longueur la longueur du tableau
 * @param largeur la largeur du tableau
 */
Tableau2D::Tableau2D( unsigned int longueur, unsigned int largeur )
{
	resize( longueur, largeur );
}

/**
 * @brief Fonction d'attribution du vector.
 * Vérification si les paramètres uiLongueurTab et uiLargeurTab
 * correspondent à la taille du vector en paramètres.
 * @param vect La référence constante du vector(unsigned char) à
 * attribuer à la classe.
 * @param uiLongueurTab La longueur à attribuer au vector
 * @param uiLargeurTab La largeur à attribuer au vector
 * @return Renvoie false si les paramètres de la longueur et de la
 * largeur ne correspondent pas avec la taille du vector
 * true sinon.
 */
bool Tableau2D::bAttribuerTab( const std::vector< unsigned char > &vect,
							   unsigned int uiLongueurTab, unsigned int uiLargeurTab )
{
	if( uiLongueurTab * uiLargeurTab != vect.size() )
		return false;
	resize( uiLongueurTab, uiLargeurTab );
	std::copy ( vect.begin(), vect.end(), mVectChar.begin() );
	return true;
}

bool Tableau2D::bAttribuerTab( std::ifstream &flux, unsigned int uiLongueurTab, unsigned int uiLargeurTab )
{
	bool final = true;
	unsigned int uiMemTile, cmpt = 0;
	if( ! flux )return false;
	muiLongueurTab = uiLongueurTab;
	muiLargeurTab = uiLargeurTab;

	mVectChar.resize( muiLongueurTab * muiLargeurTab );
	//resize du tableau
	for( unsigned int i = 0; i < mVectChar.size();++i )
	{
		flux >> uiMemTile;
		if( uiMemTile > 254 )
		{//verif si la valeur rentre dans un char
			final = false;
			break;
		}

		mVectChar[ cmpt ] = static_cast< unsigned char >( uiMemTile ) ;
		++cmpt;
	}
	flux.close();
	return final;

}


/**
 * @brief Renvoie une référence constante du vector contenu dans
 * la classe
 * @return La référence constante de mVectChar.
 */
const std::vector< unsigned char > &Tableau2D::getTab()const
{
	return mVectChar;
}

/**
 * @brief Fonction de modification de la taille du vector.
 * mise a jour de muiLargeurTab et muiLongueurTab
 * @param longueur La nouvelle longueur du tableau.
 * @param largeur La nouvelle largeur du tableau.
 */
void Tableau2D::resize( unsigned int longueur, unsigned int largeur )
{
    muiLargeurTab = largeur;
    muiLongueurTab = longueur;
	mVectChar.resize( longueur * largeur  );
}

/**
 * @brief Fonction d'affichage console du tableau.
 * L'affichage sera effectué en fonction des propriétés
 * muiLargeurTab et muiLongueurTab.
 */
void Tableau2D::afficherTab()const
{
	unsigned int uiMemCaseTab;
	std::cout << "AFFICHAGE" << std::endl;
	std::cout << "Largeur tableau::" << muiLargeurTab <<
		std::endl;
	std::cout << "Longueur tableau::" << muiLongueurTab <<
		std::endl;
	for( unsigned int i = 0; i < mVectChar.size() ; ++i ){
		//conversion char vers int pour l'affichage console
		uiMemCaseTab = mVectChar[i];
		std::cout << uiMemCaseTab << " ";
		//passer a la ligne
		if( i != 0 && 0 == ( i + 1 ) % muiLongueurTab )
			std::cout << std::endl;
	}

}
bool Tableau2D::setValAt( unsigned int uiCaseX, unsigned int uiCaseY, unsigned char value )
{
	if( uiCaseX > muiLongueurTab || uiCaseY > muiLargeurTab )
	{
		return false;
	}
	mVectChar[uiCaseX + uiCaseY * muiLargeurTab] = value;
	return true;
}

void Tableau2D::reset()
{
	mVectChar.assign(mVectChar.size(), 0);
}

/**
 * @brief Récupération de la valeur a un endroit du
 * tableau donné en argument(X,Y)
 * envoie en référence constante.
 * @param uiCaseX Abscisse de la case
 * @param uiCaseY Ordonnée de la case
 * @return La valeur de la case
 */
unsigned char Tableau2D::getValAt( unsigned int uiCaseX, unsigned int uiCaseY )const{
	if( uiCaseX > muiLongueurTab || uiCaseY > muiLargeurTab ){
		return 254;
	}
	return mVectChar[ uiCaseX + uiCaseY * muiLargeurTab ];
}
