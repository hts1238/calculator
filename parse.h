#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

using namespace std;

class _Parse_ {
private:

    int i = 0;
    string s = "";

    /// Is debug messages will be showing
    bool DEBUG_LOG = 1;

    struct Stack {
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
        while (top1 != NULL) {cout << top1->field << " "; top1 = top1->next;}
        cout << "\n [ ops ] : ";
        while (top2 != NULL) {cout << (char)top2->field << " "; top2 = top2->next;}
        cout << "\n";
    }

    /// Check if Stack 'top' is empty
    bool isEmpty(Stack*& top) {
        return top == NULL;
    }

    /// Push 'elem' into Stack 'top'
    void push(Stack*& top, const double& elem) {
        Stack* pointer = (Stack*)malloc(sizeof(Stack));
        (*pointer).field = elem;
        (*pointer).next = top;
        top = pointer;
        if (DEBUG_LOG) debug();
    }

    /// Pop from Stack 'top'
    double pop(Stack*& top) {
        if (isEmpty(top)) exit(-1);
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
        exit(-1);
    }

    /// Make number from char 'ch'
    int toNum(const char& ch) {
        return ch - '0';
    }

    /// Check if char 'ch' is number
    bool isNum(const char& ch) {
        return 0 <= toNum(ch) && toNum(ch) <= 9;
    }

    /// Get priority of the operator 'ch'
    int priority(char ch) {
        if (ch == ')') return 0;
        if (ch == '+') return 1;
        if (ch == '-') return 1;
        if (ch == '*') return 2;
        if (ch == '/') return 2;
        if (ch == '^') return 3;
        if (ch == '(') return 99;

        cout << "Compilition error; Illegal operator '" << (int)ch << "'";
        _break_();
        return 0; // This line will be never called
    }

    /// Calculate two latest numbers from 'res' using operator 'op'
    double doit (char op) {
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
        if (op == '/') return a / b;
        if (op == '^') return pow(a, b);

        return 0; // This line will be never called
    }

    /// Formatting expretion
    string formatting(const string& s) {
        string S = "";
        char prev_ch = -1;
        int brackets = 0;

        if (DEBUG_LOG) cout << "\n === Start Debug log === \n\n";



        if (brackets != 0) {
            cout << "Wrong bracket sequence";
            i--;
            _break_();
        }

        for (i = 0; i < s.length(); i++) {
            char ch = s[i];
            if (ch == ' ') {
                continue;
            }
            if (ch == '(') brackets++;
            if (ch == ')') {
                if (brackets == 0 || prev_ch == '(') {
                    cout << "Compilition error! Wrong bracket sequence";
                    _break_();
                }
                brackets--;
            }

            if (prev_ch == -1) {
                if (ch == '-') {
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
                } else if (!isNum(prev_ch)) {
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
            S += ch;
            prev_ch = ch;
        }

        return S;
    }

public:
    double parse(const string& _s, const bool& _DEBUG_LOG) {
        s = _s; DEBUG_LOG = _DEBUG_LOG;

        s = formatting(s);
        
        if (DEBUG_LOG) cout << s << "\n";

        if (s.length() == 0) {
            cout << "Expretion is empty";
            _break_();
        }

        /// Main parsing
        for (i = 0; i < s.length(); i++) {
            char ch = s[i];

            if (isNum(ch)) {

                if (i != 0 && isNum(s[i-1])) {
                    (*res).field *= 10;
                    (*res).field += toNum(ch);
                    if (DEBUG_LOG) debug();
                } else {
                    push(res, toNum(ch));
                }

            } else {

                int priority_ch = priority(ch);

                if (isEmpty(ops) || priority_ch > priority((*ops).field) || ((*ops).field == '(' && ch != ')')) {
                    push(ops, ch);
                } else {

                    if (ch == ')') {
                        while ((*ops).field != '(') {
                            push(res, doit(pop(ops)));
                        }
                        pop(ops);

                    } else {
                        while (!isEmpty(ops) && (*ops).field != '(' && priority_ch <= priority((*ops).field)) {
                            push(res, doit(pop(ops)));
                        }
                        push(ops, ch);
                    }
                }
            }
        }

        if (DEBUG_LOG) cout << "End reading line\n";

        while (ops != NULL) {
            push(res, doit(pop(ops)));
        }

        if (DEBUG_LOG) cout << "\n === End Debug log ===\n\n\n";

        if ((*res).next != NULL) {
            cout << "Something is wrong, sorry ((";
            _break_();
        }

        return (*res).field;
    }
};

_Parse_ Parse;hh


#endif // PARSE_H_INCLUDED
