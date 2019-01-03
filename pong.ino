
#include "Arduino.h"

#include "LedControl.h"
#include "NonBlockingRtttl.h"

#include "pong.h"
#include "piirto.h"
#include "input.h"
#include "geometria.h"

void Pong::aja(LedControl* lc)
{
    randomSeed(analogRead(A0));
    
    Kiintea     pallo;
    Kiintea     este;
    Kiintea     seinat[SEINAT_LKM];
    Kiintea     mailat[MAILAT_LKM];
    Nelikulmio  maalit[MAALIT_LKM];

    int kiinteidenLkm = MAILAT_LKM + SEINAT_LKM + 1;
    Kiintea*    kiinteat[kiinteidenLkm];
    Kiintea*    p1 = 0;
    Kiintea*    p2 = 0;

    //mikrosekunteja
    unsigned long pelattuAika = 0;
    
    unsigned long nykyinenAika = micros();
    unsigned long edellinenAika = nykyinenAika;
    unsigned long deltaAika = 0;
    
    unsigned long pallonLiikeAika = 1000000. / 4.;
    unsigned long viimeKiihtyvyys = 0;

    unsigned long estePaivitysAika = 1000000;
    
    //bittijono pelaajasyötteelle.
    int pelaajaSyote = 0;

    //piirretään näytolle alkukuva, joka lähtee pois kun painetaan A-näppäintä ohjaimella
    for(int i = 0; i < NAYTTOJEN_LKM; i++) {
        for(int j = 0; j < NAYTON_KORKEUS; j++) {
            lc->setRow(i, j, alkukuva[NAYTON_KORKEUS * i + j]);
        }
    }
    while(!(Input::luePelaajaSyote() & PelaajaSyote::A));
    
    alusta(&pallo, &este, seinat, mailat, maalit, kiinteat, &p1, &p2);
    edellinenAika = nykyinenAika = micros();
    
    while(!tarkistaMaalit(&pallo, maalit)) { // häviöehto
        edellinenAika = nykyinenAika;
        nykyinenAika = micros();
        
        if(edellinenAika > nykyinenAika) { // jos micros(), ja täten nykyinenAika, ylivuotaa, otetaan modia edellinenAika-muuttujasta. HUOM. kääntyykö double hyvin unsigned longiksi?
            edellinenAika %= (unsigned long)pow(2, sizeof(unsigned long) * 8);
        }
        
        deltaAika = nykyinenAika - edellinenAika;
        pelattuAika += deltaAika;
        
        pelaajaSyote = Input::luePelaajaSyote();
        kasitteleSyote(pelaajaSyote, p1);
        liikutaMailaa(p1, kiinteat, kiinteidenLkm, deltaAika);        
        liikutaEstetta(&este, deltaAika, &estePaivitysAika, pallo.kulmio.y);
        liikutaPalloa(&pallo, kiinteat, kiinteidenLkm, deltaAika, &pallonLiikeAika, &viimeKiihtyvyys);
        if(rtttl::isPlaying()) rtttl::play();
        Piirto::piirra(lc, kiinteat, kiinteidenLkm);
    }
    
    if(pelattuAika > 22000000) {
        rtttl::begin(piezoPin, voittoAani);
    }
    else {
        rtttl::begin(piezoPin, havioAani);
    }
    while(rtttl::isPlaying()) rtttl::play();
}

void Pong::alusta(Kiintea *pallo, Kiintea* este, Kiintea seinat[], Kiintea mailat[], Nelikulmio maalit[], Kiintea *kiinteat[], Kiintea** p1, Kiintea** p2) {
    *pallo      = luoKiintea(luoNelikulmio(4, 3, 1, 1), luoPiste(-1, -1));
    *este       = luoKiintea(luoNelikulmio(0, 4, 8, 1), luoPiste(0, 0));
    
    seinat[0]   = luoKiintea(luoNelikulmio(0, 0, 8, 1));
    seinat[1]   = luoKiintea(luoNelikulmio(-1, 0, 1, 16));
    seinat[2]   = luoKiintea(luoNelikulmio(8, 0, 1, 16));
    
    mailat[0]   = luoKiintea(luoNelikulmio(2, 15, 2, 1));
    
    maalit[0]   = luoNelikulmio(0, 15, 8, 1);

    kiinteat[0] = &(seinat[0]);
    kiinteat[1] = &(seinat[1]);
    kiinteat[2] = &(seinat[2]);
    kiinteat[3] = *p1 = &(mailat[0]);
    kiinteat[4] = pallo;
    kiinteat[5] = este;
}

void Pong::kasitteleSyote(int pelaajaSyote, Kiintea* p1) {
    p1->v.x = p1->v.y = 0;
    
    //reagoidaan syotteeseen
    if(pelaajaSyote & PelaajaSyote::Vasen) {
        p1->v.x -= 1; // 8 ruutua sekuntissa
    }
    if(pelaajaSyote & PelaajaSyote::Oikea) {
        p1->v.x += 1;
    }
}

void Pong::liikutaPalloa(Kiintea *pallo, Kiintea *kiinteat[], int kiinteidenLkm, unsigned long deltaAika, unsigned long* pallonLiikeAika, unsigned long* kiihtyvyysAika) {
    pallo->viimePaivitys += deltaAika;
    *kiihtyvyysAika += deltaAika;

    // uusi liikeaika joka 2. sekunti
    unsigned long liikeAjat[] = { 250000,
                                  166667,
                                  125000,
                                  100000,
                                  83333,
                                  71429,
                                  62500,
                                  55556,
                                  50000,
                                  45455,
                                  41667};
    

    unsigned long indeksi = min(*kiihtyvyysAika / 2000000, 9);
    
    while(pallo->viimePaivitys > liikeAjat[indeksi]) {

        pallo->viimePaivitys -= liikeAjat[indeksi];
        
        //väliaikaiset hitboxit pallon x-, y- ja kokonaisnopeudensuuntaan
        Nelikulmio tmpxy, tmpx, tmpy;
        bool tormaysx, tormaysy, tormays;
    
        tormaysx = tormaysy = tormays = false;
    
        tmpxy = tmpx = tmpy = pallo->kulmio;
        tmpx.x += pallo->v.x;
        tmpy.y += pallo->v.y;
        tmpxy.x += pallo->v.x;
        tmpxy.y += pallo->v.y;
    
        //testataan törmäisikö pallo, jos se liikkuisi x-,y- tai kokonaisnopeuden suuntaan
        for(int i = 0; i < kiinteidenLkm; i++) {
            if(pallo->tunnus != kiinteat[i]->tunnus) {
                if(leikkaa(tmpxy, kiinteat[i]->kulmio)) {
                    tormays = true;
                }
                if(leikkaa(tmpx, kiinteat[i]->kulmio)) {
                    tormaysx = true;
                }
                if(leikkaa(tmpy, kiinteat[i]->kulmio)) {
                    tormaysy = true;
                }
            }
        }
    
        //äänieffekti
        if(tormays || tormaysx || tormaysy) {
            rtttl::begin(piezoPin, pallonAani);
        }
    
        //if-puu, jossa muutetaan pallon nopeutta törmäyksen mukaan.
        if(tormays) {
            if(tormaysx && !tormaysy) {
                pallo->v.x = -pallo->v.x;
            }
            else if(!tormaysx && tormaysy) {
                pallo->v.y = -pallo->v.y;
            }
            else {                          //tormaysx ja tormaysy tai !tormaysx ja !tormaysy
                pallo->v.x = -pallo->v.x;
                pallo->v.y = -pallo->v.y;
            }
        }
        else {
            if(tormaysx && !tormaysy) {
                pallo->v.x = -pallo->v.x;
            }
            else if (!tormaysx && tormaysy) {
                pallo->v.y = -pallo->v.y;
            }
            else if(tormaysy && tormaysx) {
                pallo->v.x = -pallo->v.x;
                pallo->v.y = -pallo->v.y;
            }
        }
    
        //liikutetaan palloa
        pallo->kulmio.x += pallo->v.x;
        pallo->kulmio.y += pallo->v.y;
    }
}

void Pong::liikutaMailaa(Kiintea* maila, Kiintea* kiinteat[], int kiinteidenLkm, unsigned long deltaAika) {
    unsigned long paivitysAika = 62500;

    maila->viimePaivitys += deltaAika;

    if(maila->viimePaivitys > paivitysAika) {
        maila->viimePaivitys -= paivitysAika;
        
        Kiintea tmp = *maila;
        bool tormays = false;
    
        tmp.kulmio.x += tmp.v.x;
        tmp.kulmio.y += tmp.v.y;
    
        // testataan aiheittaisiko liike törmäyksen
        for(int i = 0; i < kiinteidenLkm && !tormays; i++) {
            if(tmp.tunnus != kiinteat[i]->tunnus && leikkaa(tmp.kulmio, kiinteat[i]->kulmio)) {
                tormays = true;
            }
        }
        
        // jos törmäystä ei tapahdu päivitetään mailan sijainti.
        if(!tormays) { 
            *maila = tmp;
        }
    }
}

void Pong::liikutaEstetta(Kiintea* este, unsigned long deltaAika, unsigned long* paivitysAika, int pallonY) {
    este->viimePaivitys += deltaAika;
    if(este->viimePaivitys > *paivitysAika) {
        if(pallonY < este->kulmio.y) {
            este->viimePaivitys = 0;
            if(este->kulmio.w == 0) {
                este->kulmio.w = 8;
                este->kulmio.h = 1;
    
                *paivitysAika = 1000000;
            }
            else {
                este->kulmio.w = 0;
                este->kulmio.h = 0;
                este->kulmio.y = random(4, 8);
                *paivitysAika = random(2, 3) * 1000000;
            }
        }
    }
}

bool Pong::tarkistaMaalit(Kiintea* pallo, Nelikulmio maalit[]) {

    // testataan onko pallo maalissa.
    for(int i = 0; i < MAALIT_LKM; i++) {
        if(leikkaa(pallo->kulmio, maalit[i])) {
            return true;
        }
    }
    return false;
}


