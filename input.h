
#ifndef INPUT_H
#define INPUT_H

//vakioita pelaaja syotteen tulkitsemiseen. esim 'pelaajasyote & Ylos' palauttaa nollan jos painokytkin on auki ja kokonaisluvun, jos se on kiinni.
enum PelaajaSyote {
    Ylos  = 1 << 3, // 0000 1000
    Vasen = 1 << 4,
    Oikea = 1 << 5,
    Alas  = 1 << 6,
    A     = 1 << 7
};

namespace Input
{

// jos halutaan käyttää useampaa piiriä, pitää muokata koodia
const int NUM_OF_DPINS = 8; //8 pinnia per lutikka
const int PULSE_PERIOD = 1; //mikrosekkaa

int luePelaajaSyote();

} //input

#endif

