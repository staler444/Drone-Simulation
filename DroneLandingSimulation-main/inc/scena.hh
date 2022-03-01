#ifndef _SCENA_HH_
#define _SCENA_HH_

#include<bits/stdc++.h>
#include<unistd.h>
#include"Dr3D_gnuplot_api.hh"
#include"Draw3D_api_interface.hh"

#define pb push_back

#include"wektor.hh"
#include"macierz.hh"
#include"figures.hh"
#include"drone.hh"
#include"przeszkoda.hh"


using namespace std;
using drawNS::Point3D;
using drawNS::APIGnuPlot3D;

const int PROMIEN = 12;

bool doOverlap(Pointt l1, Pointt r1, Pointt l2, Pointt r2) 
{
    if (l1.x > r2.x || l2.x > r1.x) return false;
    if (l1.y < r2.y || l2.y < r1.y) return false;
    return true;
}

class scena 
{
    drawNS::Draw3DAPI *api;
    vector<pair<pair<int,shape*>,string>> ids; //wektor trzymajacy wszystkie elementy sceny
    vector<drone*> drony; 
    shape* global;
    vector<pair<Pointt,Pointt> > rects; // wektor trzymajacy odpowiadajace prostokaty
    public:

    scena(drawNS::Draw3DAPI *api, shape* xd): api(api){
        global = xd;
    }

    void dodajElement(int x)
    {
        if(x == 1)
        {
            GoraSpiczasta* xd = new GoraSpiczasta(global); 
            ids.pb({{0,xd},"GoraSpiczasta"});
            ids[(int)ids.size()-1].first.first = ids[(int)ids.size()-1].first.second->rysuj(api);
            rects.pb(xd->getRect());
        } else if(x == 2)
        {
            GoraDachowa* xd = new GoraDachowa(global); 
            ids.pb({{0,xd},"GoraDachowa"});
            ids[(int)ids.size()-1].first.first = ids[(int)ids.size()-1].first.second->rysuj(api);
            rects.pb(xd->getRect());
        } else if(x == 3)
        {
            GoraPochyla* xd = new GoraPochyla(global); 
            ids.pb({{0,xd},"GoraPochyla"});
            ids[(int)ids.size()-1].first.first = ids[(int)ids.size()-1].first.second->rysuj(api);
            rects.pb(xd->getRect());
        } else if(x == 4)
        {
            Plaskowyz* xd = new Plaskowyz(global);
            ids.pb({{0,xd},"Plaskowyz"});
            ids[(int)ids.size()-1].first.first = ids[(int)ids.size()-1].first.second->rysuj(api);
            rects.pb(xd->getRect());
        }
        vector<pair<pair<int,shape*>,string>> newids;
        for(int i = 0; i < (int)ids.size(); i++)
        {
            api->erase_shape(ids[i].first.first);
            newids.push_back(ids[i]);
            newids[(int)newids.size()-1].first.first = newids[(int)newids.size()-1].first.second->rysuj(api);
        }
        ids = newids;
        api->redraw();
    }

    void dodajDrona(drone* ws)
    {
        ids.pb({{0,ws},"Dron"});
        drony.pb(ws);
        ids[(int)ids.size()-1].first.first = ids[(int)ids.size()-1].first.second->rysuj(api);
        vector<pair<pair<int,shape*>,string>> newids;
        for(int i = 0; i < (int)ids.size(); i++)
        {
            api->erase_shape(ids[i].first.first);
            newids.push_back(ids[i]);
            newids[(int)newids.size()-1].first.first = newids[(int)newids.size()-1].first.second->rysuj(api);
        }
        double x = ws->start[0], y = ws->start[1];
        rects.push_back({Pointt(x-PROMIEN,y+PROMIEN),Pointt(x+PROMIEN,y-PROMIEN)});
        ids = newids;
        api->redraw();
    }

    void dodajLinie(double x11, double y11, double z11, double x22, double y22, double z22)
    {
        ids.pb({{0,new Linia(global,x11,y11,z11,x22,y22,z22)},"Linia"});
        ids[(int)ids.size()-1].first.first = ids[(int)ids.size()-1].first.second->rysuj(api);
        vector<pair<pair<int,shape*>,string>> newids;
        for(int i = 0; i < (int)ids.size(); i++)
        {
            api->erase_shape(ids[i].first.first);
            newids.push_back(ids[i]);
            newids[(int)newids.size()-1].first.first = newids[(int)newids.size()-1].first.second->rysuj(api);
        }
        rects.pb({Pointt(0,0),Pointt(0,0)});
        ids = newids;
        api->redraw();
    }

    void usunElement(int x)
    {
        if(x >= (int)ids.size())
        {
            cout << "Nie ma takiego elementu\n";
            return;
        }
        vector<pair<pair<int,shape*>,string>> newids;
        vector<pair<Pointt,Pointt> > newrects;
        for(int i = 0; i < (int)ids.size(); i++)
        {
            api->erase_shape(ids[i].first.first);
            if(i != x)
            {
                newids.push_back(ids[i]);
                newids[(int)newids.size()-1].first.first = newids[(int)newids.size()-1].first.second->rysuj(api);
                newrects.pb(rects[i]);
            } else {
                if(ids[i].second == "Dron")
                {
                    cout << "Nie mozna usuwac dronow\n";
                    newids.push_back(ids[i]);
                    newids[(int)newids.size()-1].first.first = newids[(int)newids.size()-1].first.second->rysuj(api);
                    newrects.pb(rects[i]);
                }
            }
        }
        rects = newrects;
        ids = newids;
        api->redraw();
    }  

    void wypiszElementy()
    {
        for(int i = 0; i < ids.size(); i++)
        {
            if(ids[i].second == "Linia") continue;
            cout << i << " " << ids[i].second << " o srodku w " << (rects[i].first.x + rects[i].second.x)/2 << " " << (rects[i].first.y + rects[i].second.y)/2 << "\n";
        }
    }


    void usunZle() // usuwa trajektorie
    {
        vector<pair<pair<int,shape*>,string>> newids;
        for(int i = 0; i < (int)ids.size(); i++)
        {
            api->erase_shape(ids[i].first.first);
            if(ids[i].second != "Linia")
            {
                newids.push_back(ids[i]);
                newids[(int)newids.size()-1].first.first = newids[(int)newids.size()-1].first.second->rysuj(api);
            }
        }
        ids = newids;
        api->redraw();
    }

    bool czyMozeLadowac(int ind)  // sprawdza czy dron o id ind moze ladowac
    {
        if(ind >= drony.size())
        {
            return true;
        }
        drone* dronik = drony[ind];
        int ile = 0;
        Pointt A(dronik->start[0] - PROMIEN,dronik->start[1] + PROMIEN),B(dronik->start[0] + PROMIEN,dronik->start[1] - PROMIEN);
        bool can = true;
        ile = 0;
        for(int i = 0; i < (int)ids.size(); i++)
        {
            auto it = ids[i];
            if(it.second == "Dron")
            {
                if(ile == ind)
                {
                    ile++;
                    continue;
                }
                can &= (!doOverlap(rects[i].first,rects[i].second,A,B));
                can &= (!doOverlap(rects[i].first,rects[i].second,B,A));
                can &= (!doOverlap(rects[i].second,rects[i].first,A,B));
                can &= (!doOverlap(rects[i].second,rects[i].first,B,A));
                ile++;
            } else if(it.second == "Linia")
            {
                continue;
            } else 
            {
                can &= (!doOverlap(rects[i].first,rects[i].second,A,B));
                can &= (!doOverlap(rects[i].first,rects[i].second,B,A));
                can &= (!doOverlap(rects[i].second,rects[i].first,A,B));
                can &= (!doOverlap(rects[i].second,rects[i].first,B,A));
            }
            if(!can)
            {
                cout << "Wykryto element " << ids[i].second << "\n"; 
                break;
            }
        }
        return can;
    }

};

#endif _SCENA_HH_