#include <bits/stdc++.h>
#include "parse.h"

using namespace std;

int main() {
    freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    cout << setprecision(12);

    string s;
    getline(cin, s);

    cout << Parse.parse(s, 1);
    return 0;
}
