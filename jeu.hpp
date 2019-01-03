#ifndef JEU_H
#define JEU_H

#include "moteur.hpp"
#include "niveau.hpp"

class Jeu
{
private:
	Moteur mMoteurPrincipal;
	Niveau mNiveau;
public:
	Jeu();
    bool chargerNiveau(unsigned int numNiv, GameMode gm);
    void initECS(GameMode mod = SOLO);
	const Niveau &getNiveau()const;
	void lancerJeu();
    const Moteur &getMainEngine()const{return mMoteurPrincipal;}
};

#endif // JEU_H
