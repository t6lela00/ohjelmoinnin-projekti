#ifndef GEOMETRIA_H
#define GEOMETRIA_H

typedef struct {
    int x;
    int y;
} Piste;

typedef struct {
    int x;
    int y;
    int w;
    int h;
} Nelikulmio;

//olio, jolla on uniikki tunnus, hitboxi ja nopeus
typedef struct {
    unsigned int tunnus;
    Nelikulmio kulmio;
    Piste v;
    unsigned long viimePaivitys;
} Kiintea;

// HOX! Yhdessä pelissä ei saa olla kuin 2^16 oliota ;-) (ylivuodon takia)
unsigned int seuraavaTunnus = 0;

unsigned int  annaTunnus();

Piste       luoPiste(int x, int y);
Nelikulmio  luoNelikulmio(int x, int y, int w, int h);
Kiintea     luoKiintea(int x, int y, int w, int h, int vx = 0, int vy = 0, unsigned long viimePaivitys = 0);
Kiintea     luoKiintea(Nelikulmio kulmio, Piste nopeus = {0, 0}, unsigned long viimePaivitys = 0);

bool        leikkaa(Piste piste1, Piste piste2);
bool        leikkaa(Piste piste, Nelikulmio kulmio);
bool        leikkaa(Nelikulmio kulmio1, Nelikulmio kulmio2);

#endif // GEOMETRIA_H
