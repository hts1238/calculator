#ifndef QPARSE_H_INCLUDED
#define QPARSE_H_INCLUDED

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "parse.h"

/// Решает квадратные уравнения
vector<double> sq(const string& s, const char& ch) {
    Parse P(s, 0);

    map<char,double> m = {{ch, 0}};
    auto l1 = P._parse_(m);

    m[ch] = 1;
    auto l2 = P._parse_(m);

    m[ch] = 2;
    auto l3 = P._parse_(m);

    vector<double> res;
    res.push_back(P._parse_(m));
    return res;
}

#endif // QPARSE_H_INCLUDED
