#include<bits/stdc++.h>
#include<unistd.h>
#include"Dr3D_gnuplot_api.hh"
#include"Draw3D_api_interface.hh"

#define pb push_back
#define st first
#define nd second

#include"inc/wektor.hh"
#include"inc/macierz.hh"
#include"inc/figures.hh"
#include"inc/drone.hh"
#include"inc/przeszkoda.hh"
#include"inc/scena.hh"

using namespace std;
using drawNS::Point3D;
using drawNS::APIGnuPlot3D;
 
int main()
{
  //srand(8);
  srand(getpid());
  // tworzymy scene i układ globalny 
  int x = 50, y = 50, z = 20;
  shape* global = new shape(); // master shape
  drawNS::Draw3DAPI* api = new APIGnuPlot3D(-x,x,-y,y, -5, z,-1);
  
  // rysujemy podłoge 
  for(int i = -y; i < y; i++) 
    api->draw_line(Point3D(-x, i, 0), Point3D(x, i, 0), "green");
  for(int i = -x; i < x; i++)
    api->draw_line(Point3D(i, -y, 0), Point3D(i, y, 0), "green");
  api->redraw();

  scena* sc = new scena(api,global);

  // tworzymy vektor dronów i pierwszego drona 
  vector<drone*> drony;
  drone* mydrone = new drone({-35,-35, 0}, global, api);
  sc->dodajElement(1);
  sc->dodajElement(2);
  sc->dodajElement(3);
  sc->dodajElement(4);
  sc->dodajDrona(mydrone);
  mydrone->build(4, 3, 2, 0.5, 8);
  mydrone->rysuj();
  drony.pb(mydrone);
  int aktywny_dron = 0; 

  //sc->dodajLinie(-35,-35,10,-15,-35,10);
  // pętla menu 
  while(true)
  {
    sc->usunZle();
    cout << "\nWhat do you want to do?\n";
    cout << "a - rotate\nb - fly ahead\nc - rotate and fly agead\nd - create a new drone\ne - change active drone\nf - add new obstaclle\ng - delete obstacle\n";
    char c; cin >> c;
    if(c == 'a')
    {
      cout << "rotate by:\n";
      double ile; cin >> ile;
      mydrone->obroc(ile);
    }
    if(c == 'b')
    {
      cout << "how many units ahead:\n";
      double ile, wysokosc; cin >> ile;
      cout << "on what height you want to flight?\n";
      cin >> wysokosc;
      vector<double> pozycjaPrzed = {mydrone->start[0],mydrone->start[1],mydrone->start[2]};
      cout << "Drawing trajectory\n";
      mydrone->lec2(ile, wysokosc);
      vector<double> pozycjaPo = {mydrone->start[0],mydrone->start[1],mydrone->start[2]};
      sc->dodajLinie(pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2],pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2] + wysokosc);
      sc->dodajLinie(pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2] + wysokosc,pozycjaPo[0],pozycjaPo[1],pozycjaPrzed[2] + wysokosc);
      sc->dodajLinie(pozycjaPo[0],pozycjaPo[1],pozycjaPo[2],pozycjaPo[0],pozycjaPo[1],pozycjaPrzed[2] + wysokosc);
      mydrone->setter(pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2]);
      mydrone->lec_up(wysokosc);
      double ile2 = ile;
      bool wyladowalem = false;
      do
      {
        mydrone->lecPrzod(ile2,wysokosc);
        pozycjaPo = {mydrone->start[0],mydrone->start[1],mydrone->start[2]};
        sc->usunZle();
        sc->dodajLinie(pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2],pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2] + wysokosc);
        sc->dodajLinie(pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2] + wysokosc,pozycjaPo[0],pozycjaPo[1],pozycjaPrzed[2] + wysokosc);
        sc->dodajLinie(pozycjaPo[0],pozycjaPo[1],pozycjaPo[2],pozycjaPo[0],pozycjaPo[1],0);
        if(sc->czyMozeLadowac(aktywny_dron)) wyladowalem = true;
        ile2 = 3;
        if(!wyladowalem) cout << "Landing impossible\n Finding nearest landing spot\n";
      } while(wyladowalem == false);
      cout << "Landing possible, landing\n";
      mydrone->lec_up(-wysokosc);
    }
    if(c == 'c')
    {
      double kat, odl, wysokosc;
      cout << "rotate by:\n";
      cin >> kat;
      cout << "how many units ahead:\n";
      cin >> odl;
      cout << "on what height you want to flight?\n";
      cin >> wysokosc;
      vector<double> pozycjaPrzed = {mydrone->start[0],mydrone->start[1],mydrone->start[2]};
      mydrone->obroc2(kat, odl, wysokosc);
      vector<double> pozycjaPo = {mydrone->start[0],mydrone->start[1],mydrone->start[2]};
      sc->dodajLinie(pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2],pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2] + wysokosc);
      sc->dodajLinie(pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2] + wysokosc,pozycjaPo[0],pozycjaPo[1],pozycjaPrzed[2] + wysokosc);
      sc->dodajLinie(pozycjaPo[0],pozycjaPo[1],pozycjaPo[2],pozycjaPo[0],pozycjaPo[1],pozycjaPrzed[2] + wysokosc);
      mydrone->setter(pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2]);
      mydrone->obroc2(360-kat,0,0);
      pozycjaPo = {mydrone->start[0],mydrone->start[1],mydrone->start[2]};
      mydrone->obroc(kat,0,0);
      mydrone->lec_up(wysokosc);
      double ile2 = odl;
      bool wyladowalem = false;
      do
      {
        mydrone->lecPrzod(ile2,wysokosc);
        pozycjaPo = {mydrone->start[0],mydrone->start[1],mydrone->start[2]};
        sc->usunZle();
        sc->dodajLinie(pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2],pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2] + wysokosc);
        sc->dodajLinie(pozycjaPrzed[0],pozycjaPrzed[1],pozycjaPrzed[2] + wysokosc,pozycjaPo[0],pozycjaPo[1],pozycjaPrzed[2] + wysokosc);
        sc->dodajLinie(pozycjaPo[0],pozycjaPo[1],pozycjaPo[2],pozycjaPo[0],pozycjaPo[1],0);
        wyladowalem |= sc->czyMozeLadowac(aktywny_dron);
        ile2 = 3;
        if(!wyladowalem) cout << "Landing impossible\n Finding nearest landing spot\n";
      } while(!wyladowalem);
        mydrone->lec_up(-wysokosc);
      }
    if(c == 'd')
    {
      double dlug = 4, szer = 3, wys = 2, R = 0.5, wys_prop = 8;
      mydrone = new drone({3, 2, 0}, global, api);
      mydrone->build(4, 3, 2, 0.5, 8);
      sc->dodajDrona(mydrone);
      drony.pb(mydrone);
      aktywny_dron = drony.size()-1;
      cout << "new drone id "  << aktywny_dron << "  \n";
      mydrone->rysuj();
      api->redraw();
    }
    if(c == 'e')
    {
      cout << "Active drones in segment" << 0 << " - " << drony.size()-1 << "\nChoose a drone\n";
      cin >> aktywny_dron;
      mydrone = drony[aktywny_dron];
      cout << "Chosen drone " << aktywny_dron << "\n";

    }
    if(c == 'f')
    {
      cout << "What element you want to add?\n1 - GoraSpiczasta\n2 - GoraDachowa\n3 - GoraPochyla\n4 - Plaskowyz\n";
      int x; cin >> x;
      if(x < 1 || x > 4)
      {
        cout << "There's no such element\n";
      } else 
      {
        sc->dodajElement(x);
      }
    }
    if(c == 'g')
    {
      cout << "What element you want to delete?\n";
      sc->wypiszElementy();
      int x; cin >> x;
      sc->usunElement(x);
    }

    if(c == 'x')
      return 0;
    if(c != 'a' and c != 'b' and c != 'c' and c != 'd' and c != 'e' and c != 'x' and c != 'f' and c != 'g')
    {
      cout << "there's no such option, type m to return to menu\n";
      c = 'x';
      while(c != 'm')
        cin >> c;
    }
  }
}
