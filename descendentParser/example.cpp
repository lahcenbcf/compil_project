#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <string>
#include <algorithm>
using namespace std;

map<string, vector<vector<string>>> grammar;
map<string, set<string>> first, follow;
map<pair<string, string>, vector<string>> parse_table;
string start_symbol;

void initialize() {
    start_symbol = "ForLoop";
    grammar = {
        {"ForLoop", {{"for", "(", "DeclarationInit", ";", "Expression", ";", "Assignment", ")", "{", "BLOCK", "}"}}},
        {"DeclarationInit", {{"TYPE", "ID", "ASSIGN", "DIGIT"}}},
        {"Expression", {{"ID", "REL_OP", "DIGIT"}}},
        {"Assignment", {{"ID", "ARITH_OP"}}},
        {"BLOCK", {{"print", "(", "ID", ")", ";"}}},
        {"TYPE", {{"int"}}},
        {"REL_OP", {{"<"}, {"<="}, {">"}, {">="}, {"=="}}},
        {"ARITH_OP", {{"+="}, {"-="}, {"++"}, {"--"}}},
        {"DIGIT", {{"0"}, {"1"}, {"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"10"}}},
        {"ID", {{"x"}, {"i"}, {"y"}, {"j"}}},
        {"ASSIGN", {{"="}}}
    };
}

void find_first() {
    bool updated;
    do {
        updated = false;
        for (auto &[non_terminal, productions] : grammar) {
            for (auto &production : productions) {
                for (auto &symbol : production) {
                    if (grammar.count(symbol) == 0) {
                        if (first[non_terminal].insert(symbol).second)
                            updated = true;
                        break;
                    } else {
                        size_t prev_size = first[non_terminal].size();
                        first[non_terminal].insert(first[symbol].begin(), first[symbol].end());
                        if (first[symbol].count("epsilon") == 0) break;
                        if (prev_size != first[non_terminal].size())
                            updated = true;
                    }
                }
            }
        }
    } while (updated);
}

void find_follow() {
    follow[start_symbol].insert("$");
    bool updated;
    do {
        updated = false;
        for (auto &[non_terminal, productions] : grammar) {
            for (auto &production : productions) {
                for (size_t i = 0; i < production.size(); ++i) {
                    if (grammar.count(production[i])) {
                        size_t prev_size = follow[production[i]].size();
                        for (size_t j = i + 1; j < production.size(); ++j) {
                            if (grammar.count(production[j]) == 0) {
                                follow[production[i]].insert(production[j]);
                                break;
                            } else {
                                follow[production[i]].insert(first[production[j]].begin(), first[production[j]].end());
                                follow[production[i]].erase("epsilon");
                                if (first[production[j]].count("epsilon") == 0) break;
                            }
                        }
                        if (i + 1 == production.size() || first[production.back()].count("epsilon")) {
                            follow[production[i]].insert(follow[non_terminal].begin(), follow[non_terminal].end());
                        }
                        if (prev_size != follow[production[i]].size())
                            updated = true;
                    }
                }
            }
        }
    } while (updated);
}

void find_table() {
    for (auto &[non_terminal, productions] : grammar) {
        for (auto &production : productions) {
            set<string> lookahead;
            for (auto &symbol : production) {
                if (grammar.count(symbol) == 0) {
                    lookahead.insert(symbol);
                    break;
                } else {
                    lookahead.insert(first[symbol].begin(), first[symbol].end());
                    if (first[symbol].count("epsilon") == 0) break;
                }
            }
            if (lookahead.count("epsilon")) {
                lookahead.erase("epsilon");
                lookahead.insert(follow[non_terminal].begin(), follow[non_terminal].end());
            }
            for (auto &terminal : lookahead) {
                parse_table[{non_terminal, terminal}] = production;
            }
        }
    }
}

void print_parse_table() {
    cout << "LL(1) Parsing Table:" << endl;
    for (auto &[key, production] : parse_table) {
        cout << "[" << key.first << ", " << key.second << "] -> ";
        for (const auto &symbol : production) {
            cout << symbol << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void parse(vector<string> tokens) {
    stack<string> st;
    st.push("$");
    st.push(start_symbol);
    size_t idx = 0;

    while (!st.empty()) {
        string top = st.top();
        if (top == "$" && idx == tokens.size()) {
            cout << "Parsing successful!" << endl;
            return;
        } else if (grammar.count(top) == 0) {
            if (top == tokens[idx]) {
                st.pop();
                ++idx;
            } else {
                cout << "Parsing error at token: " << tokens[idx] << endl;
                return;
            }
        } else {
            if (parse_table.count({top, tokens[idx]})) {
                st.pop();
                auto production = parse_table[{top, tokens[idx]}];
                for (auto it = production.rbegin(); it != production.rend(); ++it) {
                    if (*it != "epsilon") st.push(*it);
                }
            } else {
                cout << "Parsing error at token: " << tokens[idx] << endl;
                return;
            }
        }
    }

    cout << "Parsing error: stack not empty or input not fully consumed." << endl;
}

void test() {
    initialize();
    find_first();
    find_follow();
    find_table();
    print_parse_table();

    vector<string> test_case = {"for", "(", "int", "x", "=", "10", ";", "x", "<=", "10", ";", "x", "++", ")", "{", "print", "(", "x", ")", ";", "}"};
    parse(test_case);
}

int main() {
    test();
    return 0;
}
