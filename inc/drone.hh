#ifndef _DRONE_HH_
#define _DRONE_HH_

#include<bits/stdc++.h>
#include"wektor.hh"
#include"macierz.hh"
#include"figures.hh"
#include"Dr3D_gnuplot_api.hh"
#include"Draw3D_api_interface.hh"
#include<thread>
#include<chrono>

using namespace std;
using drawNS::Point3D;
using drawNS::APIGnuPlot3D;

class drone : public shape
{
    vector<shape*> el;
    vector<int> ids;
    drawNS::Draw3DAPI *api;
    public:
    drone(initializer_list<double> spawn, shape* master, drawNS::Draw3DAPI *api): shape(spawn, master), api(api){}
    void build(double dlug, double szer, double wys, double R, double prop_wys)
    {
        shape* pom = new hull(dlug, szer, wys, this);
        el.pb(pom);
        double kat_pom = 0;
        for(int i = 0; i < 4; i++, kat_pom+=90)
        {
            shape* prop_pom = new prop(R, prop_wys, wys, this, kat_pom, 'z');
            el.pb(prop_pom);
        }
        // poprawienie wysokości respa
        start = {start[0], start[1], wys/2};
    }
    void prop_rotate()
    {
        for(int i = 1; i < 5; i++)
            el[i]->rotate(15, 'z');
    }
    int rysuj() // usuwa stare obiekty i rysuje nowe
    {
        for(auto it : ids)
            api->erase_shape(it);
        for(int i = 0; i < el.size(); i++)
            ids.pb(el[i]->rysuj(api));
        api->redraw();
    }


    // interfejc dorna 
    void lec_up(double ile) // lecimy dronem do góry albo w dół zależy czy ile < 0 czy ile > 0
    {
        double pom_up = 0.1; // pom_up stała co ile przesuwammy pomiędzy klatkami 
        int spanko = 20; // ile w milisekundach czekamy po klatce
        if(ile < 0) pom_up = -pom_up;

        for(;abs(ile) >= abs(pom_up); ile-=pom_up)
        {
            move_up(pom_up);
            prop_rotate();
            rysuj();
            this_thread::sleep_for(chrono::milliseconds(spanko));
        }
        if(ile)
        {
            move_up(ile);
            prop_rotate();
            rysuj();
            this_thread::sleep_for(chrono::milliseconds(spanko));
        }
    }

    void lec_up2(double ile) // lecimy dronem do góry albo w dół zależy czy ile < 0 czy ile > 0
    {
        double pom_up = 0.1; // pom_up stała co ile przesuwammy pomiędzy klatkami 
        int spanko = 1; // ile w milisekundach czekamy po klatce
        if(ile < 0) pom_up = -pom_up;

        for(;abs(ile) >= abs(pom_up); ile-=pom_up)
        {
            move_up(pom_up);
            prop_rotate();
            //rysuj();
            this_thread::sleep_for(chrono::milliseconds(spanko));
        }
        if(ile)
        {
            move_up(ile);
            prop_rotate();
            //rysuj();
            this_thread::sleep_for(chrono::milliseconds(spanko));
        }
    }

    void lecPrzod(double dl, double wysokosc_przelotowa = 10)
    {
        double pom_forward = 0.2; // stała o ile przesuwamy między klatkami jak leci do przodu
        int spanko = 20; // ile w milisekundach czekamy między klatkami 
        for(;dl > 0; dl-=pom_forward)
        {
            move_forward(min(pom_forward, dl));
            prop_rotate();
            rotate(10, 'y');
            rysuj();
            rotate(-10, 'y');
            this_thread::sleep_for(chrono::milliseconds(spanko));
        }
    }

    void lec(double dl, double wysokosc_przelotowa = 10)
    {
        lec_up(wysokosc_przelotowa);
        lecPrzod(dl,wysokosc_przelotowa);
        lec_up(-wysokosc_przelotowa);
    }

    void lec2(double dl, double wysokosc_przelotowa = 10)
    {
        double pom_forward = 0.2; // stała o ile przesuwamy między klatkami jak leci do przodu
        int spanko = 1; // ile w milisekundach czekamy między klatkami 
        lec_up2(wysokosc_przelotowa);
        for(;dl > 0; dl-=pom_forward)
        {
            move_forward(min(pom_forward, dl));
            prop_rotate();
            rotate(10, 'y');
            //rysuj();
            rotate(-10, 'y');
            this_thread::sleep_for(chrono::milliseconds(spanko));
        }
        lec_up2(-wysokosc_przelotowa);
    }

    void obroc(double kat, double dl = 0, double wysokosc_przelotowa = 10)
    {
        double pom, podlatuj_przy_rotacji = 2;
        int spanko = 20;
        if(kat < 0) pom = -5;
        else pom = 5;
        
        lec_up(podlatuj_przy_rotacji);
        for(;abs(kat) >= abs(pom); kat -= pom)
        {
            rotate(pom, 'z');
            prop_rotate();
            rysuj();
            this_thread::sleep_for(chrono::milliseconds(spanko));
        }
        if(kat)
        {
            rotate(kat, 'z');
            prop_rotate();
            rysuj();
            this_thread::sleep_for(chrono::milliseconds(spanko));
        }
        if(dl)
            lec(dl, wysokosc_przelotowa-podlatuj_przy_rotacji);
            
        lec_up(-podlatuj_przy_rotacji);
    }

    void obroc2(double kat, double dl = 0, double wysokosc_przelotowa = 10)
    {
        double pom, podlatuj_przy_rotacji = 2;
        int spanko = 1;
        if(kat < 0) pom = -5;
        else pom = 5;
        
        lec_up2(podlatuj_przy_rotacji);
        for(;abs(kat) >= abs(pom); kat -= pom)
        {
            rotate(pom, 'z');
            prop_rotate();
            //rysuj();
            this_thread::sleep_for(chrono::milliseconds(spanko));
        }
        if(kat)
        {
            rotate(kat, 'z');
            prop_rotate();
            //rysuj();
            this_thread::sleep_for(chrono::milliseconds(spanko));
        }
        if(dl)
            lec2(dl, wysokosc_przelotowa-podlatuj_przy_rotacji);
            
        lec_up2(-podlatuj_przy_rotacji);
    }

    void setter(double x, double y, double z)
    {
        start = {x,y,z};
    }
};

#endif _DRONE_HH_