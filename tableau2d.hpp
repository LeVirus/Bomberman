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
    std::vector<unsigned char> mVectChar;
    unsigned int muiLargeurTab, muiHauteurTab;
public:
	Tableau2D() = default;
    Tableau2D(unsigned int largeur,
               unsigned int hauteur);
	void reset();
    bool bAttribuerTab(const std::vector<unsigned char> &vect, unsigned int uiLargeurTab, unsigned int uiHauteurTab);
    bool bAttribuerTab(std::ifstream &flux, unsigned int uiLargeurTab, unsigned int uiHauteurTab);

    unsigned char getValAt(unsigned int uiCaseX, unsigned int uiCaseY)const;
    bool setValAt(unsigned int uiCaseX, unsigned int uiCaseY, unsigned char value);

	const std::vector< unsigned char > &getTab()const;
    void resize(unsigned int largeur , unsigned int hauteur);

    void addToAllCase(unsigned char val);
    inline unsigned int getLargeur()const{return muiLargeurTab;}
    inline unsigned int getHauteur()const{return muiHauteurTab;}

    void afficherTab()const;
    void substractToAllCase(unsigned char val);
};

#endif // TABLEAU2D_H
