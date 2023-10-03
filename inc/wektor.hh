#ifndef _WEKTOR_HH_
#define _WEKTOR_HH_

#include <bits/stdc++.h>
#include <initializer_list>
#include "macierz.hh"

using namespace std;

struct Pointt {
    double x, y;
    Pointt(double xx, double yy): x(xx), y(yy) {}
};

template<int roz>
class wektor {
private:
    vector<double> v;
public: 
    wektor(): v(vector<double>(roz)){}
    wektor(initializer_list<double> v2): v(v2) {}

    double operator[](int ind) const { return v[ind]; }

    wektor<roz> operator+(const wektor<roz> v2) const {
      wektor<roz> res;
      for(int i = 0; i < roz; i++)
        res.v[i] = v[i]+v2.v[i];
      return res;
    }

    wektor<roz> operator-(const wektor<roz> v2) const {
      wektor<roz> res;
      for(int i = 0; i < roz; i++)
        res.v[i] = v[i]-v2.v[i];
      return res;
    }

    wektor<roz> operator*(const mx<roz> m2) const {
      wektor<roz> res;
      for(int i = 0; i < roz; i++)
        for(int j = 0; j < roz; j++)
          res.v[i] += m2.m[i][j]*v[j];
      return res;
    }

    friend ostream& operator<<(ostream &os, const wektor<roz> & v2) {
      os << "[ ";
      for(auto it : v2.v)
        os << fixed << it << ", ";
      os << ']';
      return os;
    }
};

template<int roz>
double dl(wektor<roz> v1, wektor<roz> v2) {
  double res = 0;
  v1 = v1-v2;
  for(int i = 0; i < roz; i++)
    res+=v1[i]*v1[i];
  return sqrt(res);
}

#endif /* _WEKTOR_HH_ */
