#ifndef JEU_H
#define JEU_H

#include "moteur.hpp"
#include "niveau.hpp"

class Jeu
{
private:
	Moteur mMoteurPrincipal;
	Niveau mNiveau;
    static GameMode mGameMode;
public:
    Jeu(GameMode gm);
    bool chargerNiveau(unsigned int numNiv);
    void initECS();
	const Niveau &getNiveau()const;
	void lancerJeu();
    const Moteur &getMainEngine()const{return mMoteurPrincipal;}
    static GameMode getGameMode(){return mGameMode;}
};

#endif // JEU_H
