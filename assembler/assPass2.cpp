#include <bits/stdc++.h>
using namespace std;

class Tables {
    vector<pair<string, string>> SYMBTAB;
    vector<pair<string, string>> LITTAB;

    public:
    Tables() {
        fstream symb_tab;
        symb_tab.open("symbol_table.txt", ios::in);
        if (symb_tab.is_open()) {
            string line;
            while(!symb_tab.eof()) {
                string key;
                getline(symb_tab, line);
                string w = "";
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] != ' ') {
                        w += line[i];
                    } else {
                        key.assign(w);
                        w = "";
                    }
                }
                SYMBTAB.push_back(pair<string, string>(key, w));
            }
        } else {
            cout<<"Error opening file"<<endl;
        }

        fstream lit_tab;
        lit_tab.open("literal_table.txt", ios::in);
        if (lit_tab.is_open()) {
            string line;
            while(!lit_tab.eof()) {
                string key;
                getline(lit_tab, line);
                string w = "";
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] != ' ') {
                        w += line[i];
                    } else {
                        key.assign(w);
                        w = "";
                    }
                }
                LITTAB.push_back(pair<string, string>(key, w));
            }
        } else {
            cout<<"Error opening file"<<endl;
        }
    }
    friend class Assembler;
};

class Assembler {
    Tables t;
    fstream intCode;
    fstream file;

    public:
    Assembler() {
        intCode.open("intermediate_code.txt", ios::in);
        if (!intCode.is_open()) {
            cout<<"Error opening file"<<endl;
        }

        file.open("output.txt", ios::out);
        if (!intCode.is_open()) {
            cout<<"Error opening file"<<endl;
        }
    }

    void passTwo() {
        string line;
        vector<string> words;
        if (intCode.is_open()) {
            while (!intCode.eof()) {
                getline(intCode, line);
                string w = "";
                for (int i = 0;i < line.size(); i++) {
                    if (line[i] != ' ') {
                        w += line[i];
                    } else {
                        words.push_back(w);
                        w = "";
                    }
                }
                words.push_back(w);

                if (words[1][1] == 'I' && words[1].at(2) == 'S') {
                    file<<words[0]<<" "<<words[2].at(0);
                    if (words[2].at(1) != ')') {
                        file<<words[2].at(1)<<" ";
                    } else {
                        file<<" ";
                    }

                    if (words.size() == 5) {
                        if (words[3].at(1) == 'S') {
                            int index = words[4].at(0) - '0';
                            file<<t.SYMBTAB[index - 1].second;
                        } else {
                            file<<words[3].at(1)<<" "<<words[4].at(1);
                        }
                    } else if (words.size() == 6) {
                        file<<words[3].at(1)<<" ";
                        if (words[4].at(1) == 'S') {
                            int index = words[5].at(0) - '0';
                            file<<t.SYMBTAB[index - 1].second;
                        } else if (words[4].at(1) == 'L') {
                            int index = words[5].at(0) - '0';
                            file<<t.LITTAB[index - 1].second;
                        }
                    }
                    file<<endl;
                }
                words.clear();
            }
        }
    }
};

int main() {
    Assembler a;
    a.passTwo();
}