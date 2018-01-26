#include "tableau2d.hpp"
#include <iostream>


/**
 * @brief Constructeur de la classe Tableau2D
 * Définition de la longueur et de la largeur du tableau
 * @param longueur la longueur du tableau
 * @param largeur la largeur du tableau
 */
Tableau2D::Tableau2D(unsigned int largeur, unsigned int hauteur)
{
    resize(largeur, hauteur);
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
bool Tableau2D::bAttribuerTab(const std::vector< unsigned char > &vect,
                               unsigned int uiLargeurTab, unsigned int uiHauteurTab)
{
    if( uiLargeurTab * uiHauteurTab != vect.size() )
		return false;
    resize( uiLargeurTab, uiHauteurTab );
	std::copy ( vect.begin(), vect.end(), mVectChar.begin() );
	return true;
}

bool Tableau2D::bAttribuerTab(std::ifstream &flux, unsigned int uiLargeurTab, unsigned int uiHauteurTab)
{
	bool final = true;
	unsigned int uiMemTile, cmpt = 0;
    if(! flux)return false;
    muiLargeurTab = uiLargeurTab;
    muiHauteurTab = uiHauteurTab;

    mVectChar.resize( muiLargeurTab * muiHauteurTab );
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
void Tableau2D::resize(unsigned int largeur, unsigned int hauteur)
{
    muiHauteurTab = hauteur;
    muiLargeurTab = largeur;
    mVectChar.resize( largeur * hauteur  );
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
    std::cout << "Largeur tableau::" << muiLargeurTab<<
		std::endl;
    std::cout << "Hauteur tableau::" << muiHauteurTab  <<
		std::endl;
	for( unsigned int i = 0; i < mVectChar.size() ; ++i ){
		//conversion char vers int pour l'affichage console
		uiMemCaseTab = mVectChar[i];
		std::cout << uiMemCaseTab << " ";
		//passer a la ligne
        if( i != 0 && 0 == ( i + 1 ) % muiLargeurTab )
			std::cout << std::endl;
	}

}
bool Tableau2D::setValAt(unsigned int uiCaseX, unsigned int uiCaseY, unsigned char value)
{
    if(uiCaseX >= muiLargeurTab || uiCaseY >= muiHauteurTab)
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
unsigned char Tableau2D::getValAt(unsigned int uiCaseX, unsigned int uiCaseY)const{
    if(uiCaseX >= muiLargeurTab || uiCaseY >= muiHauteurTab){
		return 254;
	}
    return mVectChar[uiCaseX + uiCaseY * muiHauteurTab];
}
