#include "moteurgraphique.hpp"
#include "moteur.hpp"
#include <cassert>

void MoteurGraphique::linkMainEngine( Moteur* ptrMoteur )
{
	assert( ptrMoteur && "ptrMoteur == null\n" );
	mPtrMoteurPrincipal = ptrMoteur;
}
