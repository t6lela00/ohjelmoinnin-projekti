
#ifndef PIIRTO_H
#define PIIRTO_H

class LedControl;

#include "geometria.h"

namespace Piirto
{

void        piirra(LedControl* lc, Kiintea* kiinteat[], int kiinteidenLkm);
void        piirraKuva(Kiintea *kiinteat[], int kiinteidenLkm, byte* kuva);
void        piirraNelikulmioKuvaan(byte* kuva, Nelikulmio kulmio);

}

#endif

