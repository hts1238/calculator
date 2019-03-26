#include <bits/stdc++.h>
#include "parse.h"

using namespace std;

int main() {
    freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    cout << setprecision(12);

    string s;
    while(getline(cin, s)) {
        cout << " " << s << " = " << parse(s, 1) << "\n";
    }

    return 0;
}
