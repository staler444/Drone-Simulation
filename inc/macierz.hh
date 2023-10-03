#ifndef _MACIERZ_HH_
#define _MACIERZ_HH_

#include<bits/stdc++.h>

#define vd vector<double>
#define vvd vector<vd>
#define pb push_back

using namespace std;

template<int roz>
class mx {
    public:
    vvd m;
    mx(): m(vvd(roz, vd(roz))){}
    mx(std::initializer_list<double>m2): m(vvd(roz, vd(roz))) {
        auto it = m2.begin();
        for(int i = 0; i < roz; i++)
            for(int j = 0; j < roz; j++, ++it)
                m[i][j] = (*it);
    }
    mx(bool senko): m(vvd(roz, vd(roz))) { 
        if(!senko)
            return;
        for(int i = 0; i < roz; i++) 
            m[i][i] = 1;
    }

    mx<roz> operator *(const mx<roz> m2) const {
        mx<roz> res;
        for(int i = 0; i < roz; i++)
            for(int j = 0; j < roz; j++)
                for(int k = 0; k < roz; k++)
                    res.m[i][j] += m[i][k]*m2.m[k][j];
        return res;
    }

    mx(double angle, char typ): m(vvd(roz, vd(roz))) { // x, y, z
        double cs = cos(angle*M_PI/180.0), si = sin(angle*M_PI/180.0);
        if(typ == 'x') {
            m[0] = {1, 0, 0};
            m[1] = {0, cs, -si};
            m[2] = {0, si, cs};
        }
        if(typ == 'y') {
            m[0] = {cs, 0, si};
            m[1] = {0, 1, 0};
            m[2] = {-si, 0, cs};
        }
        if(typ == 'z') {
            m[0] = {cs, -si, 0};
            m[1] = {si, cs, 0};
            m[2] = {0, 0, 1};
        }
    }
    friend ostream& operator<< (ostream &os, const mx<roz> & m2) {
        for(int i = 0; i < m2.m.size(); i++) {
            for(auto it : m2.m[i])
                os << fixed << it << ' ';
            os << endl;
        }
      return os;
    }

    void neutralna() {
        m = vvd(roz, vd(roz));
        for(int i = 0; i < roz; i++)
            m[i][i] = 1;
    }
};

#endif /* _MACIERZ_HH_ */
