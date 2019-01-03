
#include "LedControl.h"

#include "definet.h"
#include "piirto.h"
#include "geometria.h"

void Piirto::piirra(LedControl* lc, Kiintea* kiinteat[], int kiinteidenLkm) {
    byte kuva[NAYTTOJEN_LKM * NAYTON_KORKEUS] = {B0};

    for(int i = 0; i < kiinteidenLkm; i++) {
        piirraNelikulmioKuvaan(kuva, kiinteat[i]->kulmio);
    }
    
    for(int i = 0; i < NAYTTOJEN_LKM; i++) {
        for(int j = 0; j < NAYTON_KORKEUS; j++) {
            lc->setRow(i, j, kuva[NAYTON_KORKEUS * i + j]);
        }
    }
}

void Piirto::piirraNelikulmioKuvaan(byte* kuva, Nelikulmio kulmio) {
    for(int y = kulmio.y; y < kulmio.y + kulmio.h; y++) {
        for(int x = kulmio.x; x < kulmio.x + kulmio.w; x++) {
            if(x >= 0 && x < NAYTON_LEVEYS && y >= 0 && y < NAYTON_KORKEUS * NAYTTOJEN_LKM) {
                kuva[y] |= B10000000 >> (x % 8);
            }
        }
    }
}
