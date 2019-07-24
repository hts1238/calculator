#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <math.h>

using namespace std;

double parse(const string& _s, const bool& _DEBUG_LOG);

class Parse {
private:
    int i;
    string s;

    /// Is debug messages will be showing
    bool DEBUG_LOG;

    struct Stack {
        bool type; /// 0 - char, 1 - number
        double field;
        Stack* next;
    };

    Stack* res = NULL;
    Stack* ops = NULL;

    /// Make and show debug message showing status of each global Stack ('res' and 'ops')
    void debug() {
        Stack* top1 = res;
        Stack* top2 = ops;
        cout << "\n [ res ] : ";
        while (top1 != NULL) {
            if (top1->type)
                cout << (double)top1->field << " ";
            else
                cout << (char)top1->field << " ";
            top1 = top1->next;
        }
        cout << "\n [ ops ] : ";
        while (top2 != NULL) {
            if (top2->type)
                cout << (double)top2->field << " ";
            else
                cout << (char)top2->field << " ";
            top2 = top2->next;
        }
        cout << "\n";
    }

    /// Check if Stack 'top' is empty
    bool isEmpty(Stack*& top) {
        return top == NULL;
    }

    /// Push 'elem' into Stack 'top'
    void push(Stack*& top, const double& elem, const bool& type) {
        Stack* pointer = (Stack*)malloc(sizeof(Stack));
        (*pointer).type = type;
        (*pointer).field = elem;
        (*pointer).next = top;
        top = pointer;
        if (DEBUG_LOG) debug();
    }

    /// Pop from Stack 'top'
    double pop(Stack*& top) {
        Stack* pointer = top;
        top = (*top).next;
        if (DEBUG_LOG) debug();
        return (*pointer).field;
    }

    /// Make and show error message and stop the programm
    void _break_() {
        cout << "\n\n";
        cout << s << "\n";
        for (int j = 0; j < i; j++) cout << " ";
        cout << "^";
        if (DEBUG_LOG) cout << "\n\n === End Debug log ===\n";
        exit(1);
    }

    /// Make number from char 'ch'
    int toNum(const char& ch) {
        return ch - '0';
    }

    /// Check if char 'ch' is number
    bool isNum(const char& ch) {
        return 0 <= toNum(ch) && toNum(ch) <= 9;
    }

    /// Make lowercase from char 'ch'
    char toLow(const char& ch) {
        return ('a' <= ch && ch <= 'z') ? ch : ch - 'A' + 'a';
    }

    /// Check if char 'ch' is letter
    bool isLet(const char& ch) {
        return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
    }

    /// Get priority of the operator 'ch'
    int priority(const char& ch) {
        if (ch == ')') return 0;
        if (ch == '+') return 1;
        if (ch == '-') return 1;
        if (ch == '*') return 2;
        if (ch == '/') return 2;
        if (ch == '^') return 3;
        if (ch == '(') return 99;

        cout << "Compilition error; Illegal operator '" << (char)ch << "'";
        _break_();
        return 0; // This line will be never called
    }

    /// Calculate two latest numbers from 'res' using operator 'op'
    double doit (const char& op) {
        if (DEBUG_LOG) cout << " " << op << "\n";
        double a, b;

        if (!isEmpty(res)) b = pop(res);
        else {
            cout << "res is empty!";
            _break_();
        }

        if (DEBUG_LOG) cout << " " << op << " " << b << "\n";

        if (!isEmpty(res)) a = pop(res);
        else {
            cout << "res is too short!";
            _break_();
        }

        if (DEBUG_LOG) cout << "\n === " << a << " " << op << " " << b << " ===\n";

        if (op == '+') return a + b;
        if (op == '-') return a - b;
        if (op == '*') return a * b;
        if (op == '/') {
            if (b == 0) {
                cout << "Time error! Division by zero!";
                _break_();
            }
            return a / b;
        }

        if (op == '^') return pow(a, b);

        return 0; // This line will be never called
    }

    /// Calculate number 'a' using function 'command'
    double doCommand(const string& command, const double& a) {
        if (command == "sqrt") return sqrt(a);
        if (command == "sin") return sin(a);
        if (command == "cos") return cos(a);
        if (command == "abs") return abs(a);
        if (command == "exp") return exp(a);
        if (command == "cbrt") return cbrt(a);
        if (command == "tan") return tan(a);
        if (command == "log") return log(a);
        //if (command == "log2") return log2(a);
        //if (command == "log10") return log10(a);
        if (command == "arcsin") return asin(a);
        if (command == "arccos") return acos(a);
        if (command == "arctan") return atan(a);

        cout << "Compilition error! Illegal function " << command;
        _break_();

        return 0;
    }

    /// Formatting expretion
    string formatting(const string& s) {
        string S = "";
        char prev_ch = -1;
        int brackets = 0;
        int modules = 0;

        if (DEBUG_LOG) cout << "\n === Start Debug log === \n\nBefore formatting: " << s << "\nAfter formatting : ";

        for (i = 0; i < s.length(); i++) {
            char ch = s[i];
            if (ch == ' ') {
                continue;
            }

            if (ch == ':') {
                ch = '/';
            }

            if (ch == '(') brackets++;

            if (ch == ')') {
                if (brackets == 0) {
                    S = '(' + S;
                    brackets++;
                }
                if (prev_ch == '(') {
                    cout << "Compilition error! Wrong bracket sequence";
                    _break_();
                }
                brackets--;
            }

            if (isLet(ch)) {
                if (isNum(prev_ch)) {
                    S += '*';
                }
                ch = toLow(ch);
            }

            if (ch == '|') {
                if (modules == 0 || (!isNum(prev_ch) && prev_ch != '|')) {
                    if (isNum(prev_ch)) {
                        S += '*';
                    }
                    S += "abs(";
                    modules++;
                } else {
                    S += ')';
                    modules--;
                }
                prev_ch = ch;
                continue;
            }

            if (prev_ch == -1) {
                if (ch == '-' || ch == '.') {
                    S += '0';
                }
                S += ch;
                prev_ch = ch;
                continue;
            }

            if (ch == '(' && (prev_ch == ')' || isNum(prev_ch))) {
                S += '*';
            }

            if (ch == '-') {
                if (prev_ch == '(') {
                    S += '0';
                } else if (!isNum(prev_ch) && prev_ch != ')' && prev_ch != '|') {
                    cout << "Compilition error! Illegal operator befor '-':";
                    i--;
                    _break_();
                }
            }

            if (ch == '+' && !isNum(prev_ch) && prev_ch != ')') {
                continue;
            }

            if (isNum(ch) && prev_ch == ')') {
                S += '*';
            }

            if (ch == '.') {
                if (prev_ch == ')') {
                    S += '*';
                }
                if (!isNum(prev_ch)) {
                    S += '0';
                }
            }

            S += ch;
            prev_ch = ch;
        }

        if (brackets != 0) {
            int bra = brackets;
            while (bra > 0) {
                S += ')';
                bra--;
            }
            while (bra < 0) {
                S = "(" + S;
                bra++;
            }

            /*cout << S << " : " << bra << "Compilition error! Wrong bracket sequence";
            _break_();*/
        }

        if (DEBUG_LOG) cout << S << "\n";

        if (S.length() == 0) {
            cout << "Expretion is empty";
            if (DEBUG_LOG) cout << "\n\n === End Debug log ===\n";
            exit(0);
        }

        return S;
    }

    /// Main parsing
    void main(const string& s) {
        for (i = 0; i < s.length(); i++) {
            char ch = s[i];

            if (isNum(ch)) {

                double num = 0;
                long long k = 10;

                while (isNum(ch)) {
                    num *= 10;
                    num += toNum(ch);
                    ch = s[++i];
                }

                if (ch == '.') {
                    ch = s[++i];
                    while (isNum(ch)) {
                        num += (double)toNum(ch) / k;
                        k *= 10;
                        ch = s[++i];
                    }
                }

                push(res, num, 1);

                if (i >= s.length()) break;
            }

            if (isLet(ch)) {
                string command = "";
                while (isLet(ch)) {
                    command += ch;
                    ch = s[++i];
                }

                string exp = "";
                if (ch == '(') {
                    exp += '(';
                    int brackets = 0;
                    ch = s[++i];
                    while (brackets >= 0) {
                        if (ch == '(') {
                            brackets++;
                        }
                        if (ch == ')') {
                            brackets--;
                        }
                        exp += ch;
                        ch = s[++i];
                    }
                } else {
                    while (isNum(ch) || ch == '.') {
                        exp += ch;
                        ch = s[++i];
                    }
                }

                push(res, doCommand(command, parse(exp, DEBUG_LOG)), 1);

                if (i >= s.length()) break;
            }

            int priority_ch = priority(ch);

            if (isEmpty(ops) || priority_ch > priority((*ops).field) || ((*ops).field == '(' && ch != ')')) {
                push(ops, ch, 0);
            } else {

                if (ch == ')') {
                    while ((*ops).field != '(') {
                        push(res, doit(pop(ops)), 1);
                    }
                    pop(ops);

                } else {
                    while (!isEmpty(ops) && (*ops).field != '(' && priority_ch <= priority((*ops).field)) {
                        push(res, doit(pop(ops)), 1);
                    }
                    push(ops, ch, 0);
                }
            }
        }

        if (DEBUG_LOG) cout << "End reading line\n";

        while (ops != NULL) {
            push(res, doit(pop(ops)), 1);
        }

        if (DEBUG_LOG) cout << "\n === End Debug log ===\n\n\n";
    }

public:
    Parse(const string& _s, const bool& _DEBUG_LOG) {
        i = 0;
        s = _s;
        DEBUG_LOG = _DEBUG_LOG;

        s = formatting(s);
    }

    double _parse_() {
        res = NULL; ops = NULL;

        main(s);

        if ((*res).next != NULL) {
            cout << "Something is wrong, sorry ((";
            _break_();
        }

        return (*res).field;
    }
};

double parse(const string& _s, const bool& _DEBUG_LOG) {
    Parse P(_s, _DEBUG_LOG);
    return P._parse_();
}

double parse(const string& _s) {
    return parse(_s, 0);
}

#endif /// PARSE_H_INCLUDED
