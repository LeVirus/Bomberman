#ifndef TABLEAU2D_H
#define TABLEAU2D_H


#include <vector>
#include <utility>
#include <fstream>

/**
 * @ class Tableau2D
 * Classe représentative d'un tableau 2 dimensions
 * Est composé d'un vector unidimentionnel.
 * (récupérer du projet ghouls.)
 */
class Tableau2D{
private:
	std::vector< unsigned char > mVectChar;
	std::pair< unsigned int, unsigned int > mPairCase;
	unsigned int muiLongueurTab, muiLargeurTab;
public:
	Tableau2D() = default;
	Tableau2D( unsigned int longueur,
			   unsigned int largeur );
	void reset();
	bool bAttribuerTab( const std::vector< unsigned char > &vect, unsigned int uiLongueurTab, unsigned int uiLargeurTab );
	bool bAttribuerTab(std::ifstream &flux, unsigned int uiLongueurTab, unsigned int uiLargeurTab );

	unsigned char getValAt( unsigned int uiCaseX, unsigned int uiCaseY )const;
	bool setValAt( unsigned int uiCaseX, unsigned int uiCaseY, unsigned char value );

	const std::vector< unsigned char > &getTab()const;
	void resize( unsigned int longueur, unsigned int largeur );

	inline unsigned int getLongueur()const{return muiLongueurTab;}
	inline unsigned int getLargeur()const{return muiLargeurTab;}
	inline unsigned int getTailleTotale()const{return muiLongueurTab * muiLargeurTab;}

	void afficherTab()const;
};

#endif // TABLEAU2D_H
