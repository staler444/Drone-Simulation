#ifndef _FIGURES_HH_
#define _FIGURES_HH_

#include"Dr3D_gnuplot_api.hh"
#include"Draw3D_api_interface.hh"
#include<bits/stdc++.h>
#include"wektor.hh"
#include"macierz.hh"

#define pb push_back

using namespace std;

using drawNS::Point3D;
using drawNS::APIGnuPlot3D;

class shape { // podstawowa klasa, tak na prawde obugująca układ współrzędnych 
public:
    wektor<3> start;
    shape *master;
    map<char, double> rotacja;

    shape(): master(NULL) {}
    shape(shape* master): start(), master(master), rotacja() {}
    shape(shape* master, double kat, char typ): start(), master(master), rotacja({{typ, kat}}){}
    shape(initializer_list<double> il, shape* master): start(il), master(master), rotacja() {}
    shape(initializer_list<double> il, shape* master, double kat, char typ): start(il), master(master), rotacja({{typ, kat}}) {}

    wektor<3> set_rotate(wektor<3> we) { // zwraca punkt przekształcony do rotacji układu 
        char t[] = {'x', 'y', 'z'};
        for(auto it : t)
            we = we*mx<3>(rotacja[it], it);
        return we;
    } 

    // przesunięcie punktu o dl w osi ox
    void move_forward(double dl) { start = start+set_rotate(wektor<3>({dl, 0, 0})); }

    // przesuwa układ o dl w osi oz
    void move_up(double dl) { start = start+set_rotate(wektor<3>({0, 0, dl})); }

    // obraca układ o zadany kąt w wokół zadanej osi
    void rotate(double kat, char typ) { rotacja[typ] += kat; }

    // przelicza współrzędne punktu na poprzedni układ 
    wektor<3> to_master(wektor<3> we) { return (master != NULL ? master->set_rotate(we)+master->start : we); }
    
    // przelicza współrzędne punktu na roota 
    wektor<3> to_root(wektor<3> we) {         
        shape* pom = this;
        while(pom != NULL) {
            we = pom->set_rotate(we)+pom->start;
            pom = pom->master;
        }
        return we;
    }

    // generuje punkty obiektu w lokalnym układzie wsp 
    virtual void points_generate() {};

    // rysuje obiekt na podstawie dwóch list numerów punktów które będą w podstawach
    int go(vector<vector<int>> t, drawNS::Draw3DAPI* api, vector<wektor<3>> &points) {
        points_generate();
        vector<vector<Point3D>> ve;
        for(auto itv : t) {
            vector<Point3D> pom;
            for(auto it : itv)
                pom.pb(Point3D(points[it]));
            ve.pb(pom);
        }
        for(size_t i = 0; i < t[0].size(); i++)
            ve.pb({Point3D(points[t[0][i]]), Point3D(points[t[1][i]])});
        return api->draw_polyhedron(ve);
    }

    // w zależności od obiektu wywołuje go z dobrze skonfigurowaną listą podstaw 
    virtual int rysuj(drawNS::Draw3DAPI* api) { return 0; }; 
};

class hull : public shape {
protected:
    double dlug, szer, wys;
    vector<wektor<3>> points;
public:
    hull(){}
    hull(double dlug, double szer, double wys, shape* master): shape(master), dlug(dlug), szer(szer), wys(wys) {}

    void points_generate() { // generuje punkty lokalnie i przelicza je na globalny układ 
        points.clear();
        double t[] = {dlug/2, szer/2, wys/2};
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++)
                for(int k = 0; k < 2; k++)
                    points.pb({(i ? t[0] : -t[0]), (j ? t[1] : -t[1]), (k ? t[2] : -t[2])});
        for(auto &it : points)
            it = to_root(it);
    }

    int rysuj(drawNS::Draw3DAPI* api) {
        return go({{0, 1, 3, 2}, {4, 5, 7, 6}}, api, points);
    }
};

class prop : public shape {
protected:
    double R, wys, hull_wys;
    vector<wektor<3>> points;
public:
    prop(){}
    prop(double R, double wys, double hull_wys, shape* master): shape(master), R(R), wys(wys), hull_wys(hull_wys) {}
    prop(double R, double wys, double hull_wys, shape* master, double kat, char typ): shape(master, kat, typ), R(R), wys(wys), hull_wys(hull_wys) {}

    void points_generate() { // generuje punkty lokalnie i przelicza je na globalny układ 
        points.clear();
        double h = R*sqrt(3)/(double)2;
        wektor<3> mid({wys/2, 0, h+hull_wys/2});
        double t[] = {wys/2, R/2, h};
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++)
                for(int k = 0; k < 2; k++)
                    points.pb({mid[0] + (i ? -t[0] : t[0]), mid[1] + (j ? -t[1] : t[1]), mid[2] + (k ? -t[2] : t[2])});
        t[0] = wys/2; t[1] = R;
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++)
                points.pb({mid[0] + (i ? -t[0] : t[0]), mid[1] + (j ? t[1] : -t[1]), mid[2]});
        for(auto &it : points)
            it = to_root(it);
    }

    int rysuj(drawNS::Draw3DAPI* api) {
        return go({{0, 9, 1, 3, 8, 2}, {4, 11, 5, 7, 10, 6}}, api, points);
    }
};

#endif /* _FIGURES_HH_ */
