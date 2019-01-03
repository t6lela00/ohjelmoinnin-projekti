
#ifndef PONG_H
#define PONG_H

#include "geometria.h"

class LedControl;

namespace Pong {

//äänieffektit nonblockingrtttl-kirjastolle
const char * pallonAani = "pallo:d=4,o=5,b=100:32a";
const char * havioAani  = "havio:d=4,o=5,b=100:e";
const char * voittoAani = "ff6_victory:d=4,o=5,b=140:32d6,32p,32d6,32p,32d6,32p,d6,a#,c6,16d6,8p,16c6,2d6";

//alkukuva
byte alkukuva[] = {B00000000,
                   B11101110,
                   B10101010,
                   B11101010,
                   B10001010,
                   B10001010,
                   B10001110,
                   B00000000,
                   B00000000,
                   B11001110,
                   B10101000,
                   B10101000,
                   B10101010,
                   B10101010,
                   B10101110,
                   B00000000};

//definet
const int SEINAT_LKM = 4;
const int MAILAT_LKM = 1;
const int MAALIT_LKM = 1;

void        aja(LedControl* lc);

void        alusta(Kiintea* pallo, Kiintea* este, Kiintea seinat[], Kiintea mailat[], Nelikulmio maalit[], Kiintea *kiinteat[], Kiintea **p1, Kiintea **p2);
void        kasitteleSyote(int pelaajaSyote, Kiintea* p1);
void        liikutaPalloa(Kiintea *pallo, Kiintea *kiinteat[], int kiinteidenLkm, unsigned long deltaAika, unsigned long* pallonLiikeAika, unsigned long* pallonKiihtyvyysAika);
void        liikutaMailaa(Kiintea* maila, Kiintea* kiinteat[], int kiinteidenLkm, unsigned long deltaAika);
void        liikutaEstetta(Kiintea* este, unsigned long deltaAika, unsigned long* paivitysAika, int pallonY);
bool        tarkistaMaalit(Kiintea* pallo, Nelikulmio maalit[]);

}

#endif

