#ifndef _PRZESZKODA_HH_
#define _PRZESZKODA_HH_

#include<bits/stdc++.h>
#include<unistd.h>
#include"Dr3D_gnuplot_api.hh"
#include"Draw3D_api_interface.hh"
#include"figures.hh"
#include"wektor.hh"
#include"macierz.hh"

#define pb push_back

using namespace std;

int rand(int a, int b) { return a+rand()%(b-a+1); }

using drawNS::Point3D;
using drawNS::APIGnuPlot3D;

class GoraSpiczasta : public shape {
    vector<wektor<3>> points;
    public:
    GoraSpiczasta(shape* master): shape(master) {
        srand(time(NULL));
        int lbx = rand(10,45), ubx = rand(10,45);
        if(lbx > ubx) 
            swap(ubx,lbx);
        ubx++;
        int lby = rand(15,40), uby = rand(15,40);
        if(lby > uby) 
            swap(uby,lby);
        uby++;
        points.pb({(double)rand(-ubx,-lbx), (double)rand(lby,uby), 0});
        points.pb({(double)rand(-ubx,-lbx), (double)rand(lby,uby), 0});

        pair<int,int> wektorNormalny = {points[0][1] - points[1][1], points[1][0] - points[0][0]}; 

        points.pb({points[1][0] + wektorNormalny.first,points[1][1] + wektorNormalny.second,0});
        points.pb({points[0][0] + wektorNormalny.first,points[0][1] + wektorNormalny.second,0});

        int xxx = 0, yyy = 0;
        for(auto it : points) {
            xxx += it[0];
            yyy += it[1];
        }
        xxx /= 4, yyy /= 4;

        points.pb({(double)xxx, (double)yyy, 7.0});
    }

    pair<Pointt,Pointt> getRect(){
        Pointt A = Pointt(points[0][0],points[0][1]);
        Pointt B = Pointt(points[2][0],points[2][1]);
        return {A,B};
    }

    int rysuj(drawNS::Draw3DAPI* api) {
        vector<int> up, down;
        for(size_t i = 0; i + 1 < points.size(); i++) {
            down.pb(i);
            up.pb(points.size() - 1);
        }
        return go({up, down}, api, points); 
    }
};

class Linia : public shape {
    vector<wektor<3>> points;
    public:
    Linia(shape* master, double x11, double y11, double z11, double x22, double y22, double z22): shape(master) {
        points.pb({x11, y11, z11});
        points.pb({x22, y22, z22});
    }
    int rysuj(drawNS::Draw3DAPI* api) {
        vector<int> up, down;
        down.push_back(0);
        up.push_back(1);
        return go({up, down}, api, points); 
    }
};

class GoraDachowa : public shape {
    vector<wektor<3>> points;
    const int XRANGE = 13, YRANGE = 20;
    public:
    GoraDachowa(shape* master): shape(master) {
        srand(time(NULL));
        points.pb({(double)rand(4,XRANGE), (double)rand(-YRANGE,-XRANGE), 0});
        points.pb({(double)rand(4,XRANGE), (double)rand(-YRANGE,-XRANGE), 0});

        pair<int,int> wektorNormalny = {3*(points[0][1] - points[1][1]), 3*(points[1][0] - points[0][0])}; 

        points.pb({points[1][0] + wektorNormalny.first,points[1][1] + wektorNormalny.second,0});
        points.pb({points[0][0] + wektorNormalny.first,points[0][1] + wektorNormalny.second,0});

        int xxx = 0, yyy = 0;
        for(auto it : points) {
            xxx += it[0];
            yyy += it[1];
        }
        xxx /= 4, yyy /= 4;

        points.pb({(double)xxx, (double)yyy, 7.0});
    }

    pair<Pointt,Pointt> getRect() {
        Pointt A = Pointt(points[0][0],points[0][1]);
        Pointt B = Pointt(points[2][0],points[2][1]);
        return {A,B};
    }

    int rysuj(drawNS::Draw3DAPI* api) {
        vector<int> up, down;
        for(size_t i = 0; i + 1 < points.size(); i++) {
            down.pb(i);
            up.pb(points.size() - 1);
        }
        return go({up, down}, api, points); 
    }
};

class GoraPochyla : public shape {
    vector<wektor<3>> points;
    const int XRANGE = 25, YRANGE = 28, HEIGHT = 4;
    public:
    GoraPochyla(shape* master): shape(master) {
        srand(time(NULL));
        points.pb({(double)rand(4,XRANGE), (double)rand(-YRANGE,-XRANGE), 0.0});
        points.pb({(double)rand(4,XRANGE), (double)rand(-YRANGE,-XRANGE), 0.0});

        pair<int,int> wektorNormalny = {(points[0][1] - points[1][1]), (points[1][0] - points[0][0])}; 

        points.pb({points[1][0] + wektorNormalny.first,points[1][1] + wektorNormalny.second,0});
        points.pb({points[0][0] + wektorNormalny.first,points[0][1] + wektorNormalny.second,0});

        points.pb(points[0]);
        points.pb(points[1]);

        points.pb({points[1][0] + wektorNormalny.first,points[1][1] + wektorNormalny.second, (double)HEIGHT});
        points.pb({points[0][0] + wektorNormalny.first,points[0][1] + wektorNormalny.second, (double)HEIGHT});

    }
    pair<Pointt,Pointt> getRect() {
        Pointt A = Pointt(points[0][0],points[0][1]);
        Pointt B = Pointt(points[2][0],points[2][1]);
        return {A,B};
    }
    int rysuj(drawNS::Draw3DAPI* api) {
        vector<int> up, down;
        for(int i = 0; i < 4; i++)
            down.pb(i);
        for(int i = 4; i < (int)points.size(); i++)
            up.pb(i);

        return go({up, down}, api, points); 
    }
};

class Plaskowyz : public shape {
    vector<wektor<3>> points;
    const int XRANGE = 15, YRANGE = 15, HEIGHT = 2;
    public:

    Plaskowyz(shape* master): shape(master) {
        srand(time(NULL));
        points.pb({(double)rand(-2*XRANGE,-XRANGE), (double)rand(-YRANGE,YRANGE), 0});
        points.pb({(double)points.back()[0], points.back()[1], (double)HEIGHT});
        points.pb({(double)rand(-2*XRANGE,-XRANGE), (double)rand(-YRANGE,YRANGE), (double)0});
        points.pb({points.back()[0], points.back()[1], (double)HEIGHT});

        pair<int,int> wektorNormalny = {points[0][1] - points[2][1], points[2][0] - points[0][0]}; 

        points.pb({points[2][0] + wektorNormalny.first,points[2][1] + wektorNormalny.second,0});
        points.pb({points.back()[0], points.back()[1], (double)HEIGHT});

        points.pb({points[0][0] + wektorNormalny.first,points[0][1] + wektorNormalny.second,0});
        points.pb({points.back()[0], points.back()[1], (double)HEIGHT});

    }

    pair<Pointt,Pointt> getRect() {
        Pointt A = Pointt(points[0][0],points[0][1]);
        Pointt B = Pointt(points[4][0],points[4][1]);
        return {A,B};
    }

    int rysuj(drawNS::Draw3DAPI* api) {
        vector<int> up, down;
        for(size_t i = 0; i + 1 < points.size(); i+=2) {
            down.pb(i);
            up.pb(i+1);
        }
        return go({up, down}, api, points); 
    }
};

#endif /* _PRZESZKODA_HH_ */
