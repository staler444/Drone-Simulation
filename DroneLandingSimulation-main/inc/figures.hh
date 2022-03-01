#ifndef _FIGURES_HH_
#define _FIGURES_HH_

#include<bits/stdc++.h>
#include"wektor.hh"
#include"macierz.hh"
#include"Dr3D_gnuplot_api.hh"
#include"Draw3D_api_interface.hh"

using namespace std;

using drawNS::Point3D;
using drawNS::APIGnuPlot3D;

class shape // podstawowa klasa, tak na prawde obugująca układ współrzędnych 
{
    public:
    wektor<3> start;
    shape *master;
    map<char, double> rotacja;
    public:
    shape(): master(NULL){}
    shape(shape* master): master(master){}
    shape(shape* master, double kat, char typ): master(master), rotacja({{typ, kat}}){}
    shape(initializer_list<double> il, shape* master):start(il), master(master){}
    shape(initializer_list<double> il, shape* master, double kat, char typ):start(il), rotacja({{typ, kat}}), master(master){}
    wektor<3> set_rotate(wektor<3> we) // zwraca punkt przekształcony do rotacji układu 
    {
        char t[] = {'x', 'y', 'z'};
        for(auto it : t)
            we = we*mx<3>(rotacja[it], it);
        return we;
    } 
    void move_forward(double dl) // przesunięcie punktu o dl w osi ox
    {  
        start = start+set_rotate(wektor<3>({dl, 0, 0}));
    }
    void move_up(double dl) // przesuwa układ o dl w osi oz
    {
        start = start+set_rotate(wektor<3>({0, 0, dl}));
    }
    void rotate(double kat, char typ) // obraca układ o zadany kąt w wokół zadanej osi
    {
        rotacja[typ] += kat;
    }
    wektor<3> to_master(wektor<3> we) // przelicza współrzędne punktu na poprzedni układ 
    {
        return (master != NULL ? master->set_rotate(we)+master->start : we);
    }
    wektor<3> to_root(wektor<3> we) // przelicza współrzędne punktu na roota 
    {
        shape* pom = this;
        while(pom != NULL)
        {
            we = pom->set_rotate(we)+pom->start;
            pom = pom->master;
        }
        return we;
    }
    virtual void points_generate(){}; // generuje punkty obiektu w lokalnym układzie wsp 
    int go(vector<vector<int>> t, drawNS::Draw3DAPI* api, vector<wektor<3>> &points) // rysuje obiekt na podstawie dwóch list numerów punktów które będą w podstawach
    {
        points_generate();
        vector<vector<Point3D>> ve;
        for(auto itv : t)
        {
            vector<Point3D> pom;
            for(auto it : itv)
                pom.pb(Point3D(points[it]));
            ve.pb(pom);
        }
        for(int i = 0; i < t[0].size(); i++)
            ve.pb({Point3D(points[t[0][i]]), Point3D(points[t[1][i]])});
        return api->draw_polyhedron(ve);
    }
    virtual int rysuj(drawNS::Draw3DAPI* api){}; // w zależności od obiektu wywołuje go z dobrze skonfigurowaną listą podstaw 
};

class hull : public shape
{
    protected:
    double dlug, szer, wys;
    vector<wektor<3>> points;
    public:
    hull(){}
    hull(double dlug, double szer, double wys, shape* master): dlug(dlug), szer(szer), wys(wys), shape(master){}
    void points_generate() // generuje punkty lokalnie i przelicza je na globalny układ 
    {
        points.clear();
        double t[] = {dlug/2, szer/2, wys/2};
        for(int i = 0; i < 2; i++)
            for(int j = 0; j < 2; j++)
                for(int k = 0; k < 2; k++)
                    points.pb({(i ? t[0] : -t[0]), (j ? t[1] : -t[1]), (k ? t[2] : -t[2])});
        for(auto &it : points)
            it = to_root(it);
    }
    int rysuj(drawNS::Draw3DAPI* api)
    {
        return go({{0, 1, 3, 2}, {4, 5, 7, 6}}, api, points);
    }
};

class prop : public shape
{
    protected:
    double R, wys, hull_wys;
    vector<wektor<3>> points;
    public:
    prop(){}
    prop(double R, double wys, double hull_wys, shape* master): R(R), wys(wys), hull_wys(hull_wys), shape(master){}
    prop(double R, double wys, double hull_wys, shape* master, double kat, char typ):R(R), wys(wys), hull_wys(hull_wys), shape(master, kat, typ){}
    void points_generate() // generuje punkty lokalnie i przelicza je na globalny układ 
    {
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
    int rysuj(drawNS::Draw3DAPI* api)
    {
        return go({{0, 9, 1, 3, 8, 2}, {4, 11, 5, 7, 10, 6}}, api, points);
    }
};

#endif _FIGURES_HH_