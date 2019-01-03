
#include "geometria.h"


unsigned int  annaTunnus() {
    return seuraavaTunnus++;
}

Piste luoPiste(int x, int y) {
    Piste piste = {x, y};
    return piste;
}

Nelikulmio luoNelikulmio(int x, int y, int w, int h) {
    Nelikulmio kulmio = {x, y, w, h};
    return kulmio;
}

Kiintea luoKiintea(int x, int y, int w, int h, int vx, int vy, unsigned long viimePaivitys) {
    Kiintea kiintea = {annaTunnus(), x, y, w, h, vx, vy, viimePaivitys};
    return kiintea;
}

Kiintea luoKiintea(Nelikulmio kulmio, Piste nopeus, unsigned long viimePaivitys) {
    Kiintea kiintea = {annaTunnus(), kulmio, nopeus, viimePaivitys};
    return kiintea;
}

bool leikkaa(Piste piste1, Piste piste2) {
    return (piste1.x == piste2.x) && (piste1.y == piste2.y);
}

bool leikkaa(Piste piste, Nelikulmio kulmio) {
    return piste.x >= kulmio.x && piste.x < kulmio.x + kulmio.w &&
           piste.y >= kulmio.y && piste.y < kulmio.y + kulmio.h;
}

bool leikkaa(Nelikulmio kulmio1, Nelikulmio kulmio2) {
    return !(kulmio1.x + kulmio1.w <= kulmio2.x ||
             kulmio1.x >= kulmio2.x + kulmio2.w ||
             kulmio1.y >= kulmio2.y + kulmio2.h ||
             kulmio1.y + kulmio1.h <= kulmio2.y );
}

