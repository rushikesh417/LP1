//============================================================================
// Name        : Assignment_2.cpp
// Author      : 31458
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <bits/stdc++.h>
using namespace std;

class Tables {
public:
	unordered_map<string, pair<string, int>> OPTAB;
	unordered_map<string, int> REG;
	unordered_map<string, int> CONDCS;

    vector<pair<string, int>> SYMBTAB;
    vector<pair<string, int>> LITTAB;
    vector<int> POOLTAB;

	Tables(){
		OPTAB["STOP"] = {"IS",0};
		OPTAB["ADD"] = {"IS",1};
		OPTAB["SUB"] = {"IS",2};
		OPTAB["MULT"] = {"IS",3};
		OPTAB["MOVER"] = {"IS",4};
		OPTAB["MOVEM"] = {"IS",5};
		OPTAB["COMP"] = {"IS",6};
		OPTAB["BC"] = {"IS",7};
		OPTAB["DIV"] = {"IS",8};
		OPTAB["READ"] = {"IS",9};
		OPTAB["PRINT"] = {"IS",10};
		OPTAB["START"] = {"AD",1};
		OPTAB["END"] = {"AD",2};
		OPTAB["ORIGIN"] = {"AD",3};
		OPTAB["EQU"] = {"AD",4};
		OPTAB["LTORG"] = {"AD",5};
		OPTAB["DC"] = {"DL",1};
		OPTAB["DS"] = {"DL",2};

		REG["AREG"] = 1;
		REG["BREG"] = 2;
		REG["CREG"] = 3;
		REG["DREG"] = 4;

		CONDCS["LT"] = 1;
		CONDCS["LE"] = 2;
		CONDCS["EQ"] = 3;
		CONDCS["GT"] = 4;
		CONDCS["GE"] = 5;
		CONDCS["ANY"] = 6;
	}

    int searchSymbol(string symbol) {
        for (int i = 0; i < SYMBTAB.size(); i++) {
            if (SYMBTAB[i].first == symbol) {
                return i;
            }
        }
        return -1;
    }

    pair<string, int> findOpcode(string str) {
        if (OPTAB.find(str) != OPTAB.end()) {
            return OPTAB[str];
        } else {
            return pair<string, int> (NULL, -1);
        }
    }
};

class Assembler {
    Tables t;
    fstream file;
    fstream intCode;
    fstream symbTab;
    fstream litTab;
    fstream poolTab;
    int counter = 0;
    int scounter = 0;
    int lcounter = 0;
    int pcounter = 0;

    public:
    Assembler() {
        file.open("input.txt", ios::in);
        if (!file.is_open()) {
            cout<<"Error opening file"<<endl;
        }

        intCode.open("intermediate_code.txt", ios::out);
        if (!intCode.is_open()) {
            cout<<"Error opening file"<<endl;
        }

        symbTab.open("symbol_table.txt", ios::out);
        if (!symbTab.is_open()) {
            cout<<"Error opening file"<<endl;
        }

        litTab.open("literal_table.txt", ios::out);
        if (!litTab.is_open()) {
            cout<<"Error opening file"<<endl;
        }

        poolTab.open("pool_table.txt", ios::out);
        if (!poolTab.is_open()) {
            cout<<"Error opening file"<<endl;
        }
    }

    void sizeOne(vector<string> words) {
        int lc = counter++;
        int temp = 0;
        pair<string, int> ic;
        if (words[0] == "LTORG") { //LTORG
            ic = t.OPTAB[words[0]];
            intCode<<lc<<" "<<"("<<ic.first<<", "<<ic.second<<")"<<endl;
            t.POOLTAB.push_back(pcounter + 1);
            t.LITTAB[pcounter].second = lc;
            for (int i = pcounter + 1; i < t.LITTAB.size(); i++) {
                lc = counter++;
                t.LITTAB[i].second = lc;
                temp = i;
            }
            pcounter = temp + 1;
        } else { //opcode
            ic = t.findOpcode(words[0]);
            intCode<<lc<<" "<<"("<<ic.first<<", "<<ic.second<<")"<<endl;
        }
    }

    void sizeTwo(vector<string> words) {
        int lc = counter++;
        int index;
        pair<string, int> ic;
        pair<string, int> ic1;
        pair<string, int> symbol;
        if (words[0] == "ORIGIN") { //ORIGIN
            ic = t.OPTAB[words[0]];
            index = t.searchSymbol(words[1]);
            counter = t.SYMBTAB[index].second;
            ic1.first = "S";
            ic1.second = index + 1;
        } else { //opcode, symbol
            ic = t.findOpcode(words[0]);
            index = t.searchSymbol(words[1]);
            ic1.first = "S";
            if (index >= 0) {
                ic1.second = index + 1;
            } else {
                scounter++;
                ic1.second = scounter;
                symbol.first = words[1];
                symbol.second = -1;
                t.SYMBTAB.push_back(symbol);
            }
        }
        intCode<<lc<<" "<<"("<<ic.first<<", "<<ic.second<<")"<<" "<<"("<<ic1.first<<", "<<ic1.second<<")"<<endl;
    }

    void sizeThree(vector<string> words) {
        int lc = counter++;
        pair<string, int> ic;
        pair<string, int> ic1;
        pair<string, int> ic2;
        pair<string, int> symbol;
        pair<string, int> literal;
        int index;
        ic = t.findOpcode(words[0]);
        if (t.REG.find(words[1]) != t.REG.end()) {
            ic1.second = t.REG[words[1]];
        } else if (t.CONDCS.find(words[1]) != t.CONDCS.end()) {
            ic1.second = t.CONDCS[words[1]];
        }
        if (t.REG.find(words[2]) != t.REG.end()) { //opcode, register, register
            ic2.second = t.REG[words[2]];
            intCode<<lc<<" "<<"("<<ic.first<<", "<<ic.second<<")"<<" "<<"("<<ic1.second<<")"<<" "<<"("<<ic2.second<<")"<<endl;
        } else if (words[2].at(0) == '=') { //opcode, register, literal
            lcounter++;
            ic2.first = "L";
            ic2.second = lcounter;
            literal.first = words[2];
            literal.second = -1;
            t.LITTAB.push_back(literal);
            intCode<<lc<<" "<<"("<<ic.first<<", "<<ic.second<<")"<<" "<<"("<<ic1.second<<")"<<" "<<"("<<ic2.first<<", "<<ic2.second<<")"<<endl;
        } else { //opcode, register, symbol
            ic2.first = "S";
            index = t.searchSymbol(words[2]);
            if (index >= 0) {
                ic2.second = index + 1;
            } else {
                scounter++;
                ic2.second = scounter;
                symbol.first = words[2];
                symbol.second = -1;
                t.SYMBTAB.push_back(symbol);
            }
            intCode<<lc<<" "<<"("<<ic.first<<", "<<ic.second<<")"<<" "<<"("<<ic1.second<<")"<<" "<<"("<<ic2.first<<", "<<ic2.second<<")"<<endl;
        }
    }

     void passOne() {
        vector<string> words;
        string line;
        int lc;
        int index;
        int temp = 0;
        pair<string, int> ic;
        pair<string, int> ic1;
        pair<string, int> ic2;
        pair<string, int> symbol;
        pair<string, int> literal;
        if (file.is_open()) {
            while (!file.eof()) {
                getline(file, line);
                string w = "";
                for (int i = 0;i < line.size(); i++) {
                    if (line[i] != ' ') {
                        w += line[i];
                    } else {
                        words.push_back(w);
                        w = "";
                    }
                }

                // cout<<words.size()<<endl;
                for(int i=0; i < words.size();i++){
                    // cout<<words[i]<<" ";
                }
                // cout<<endl;

                words.push_back(w);
                if (words[0] == "START") { //START
                    counter = stoi(words[1]);
                    ic.first = "AD";
                    ic.second = 01;
                    ic1.first = "C";
                    ic1.second = stoi(words[1]);
                    intCode<<" "<<"("<<ic.first<<", "<<ic.second<<")"<<" "<<"("<<ic1.first<<", "<<ic1.second<<")"<<endl;
                } else if (words[0] == "END") { //END
                	lc = counter++;
                    ic.first = "AD";
                    ic.second = 02;
                    intCode<<" "<<"("<<ic.first<<", "<<ic.second<<")"<<endl;
                    t.POOLTAB.push_back(pcounter + 1);
                    t.LITTAB[pcounter].second = lc;
                    for (int i = pcounter + 1; i < t.LITTAB.size(); i++) {
                    	lc = counter++;
                    	t.LITTAB[i].second = lc;
                    }
                    break;
                } else if (t.OPTAB.find(words[0]) != t.OPTAB.end()) {
                    if (words.size() == 1) {
                        this->sizeOne(words);
                    }  else if (words.size() == 2) {
                        this->sizeTwo(words);
                    } else if (words.size() == 3) {
                        this->sizeThree(words);
                    }
                } else {
                    index = t.searchSymbol(words[0]);
                    vector<string> withoutSymbol;
                    if (index >= 0) {
                        t.SYMBTAB[index].second = lc + 1;
                    } else {
                        scounter++;
                        symbol.first = words[0];
                        symbol.second = lc + 1;
                        t.SYMBTAB.push_back(symbol);
                    }
                    if (words[1] == "EQU") { //EQU
                        ic = t.OPTAB[words[1]];
                        lc = counter++;
                        ic2.first = "S";
                        index = t.searchSymbol(words[2]);
                        ic2.second = index + 1;
                        intCode<<lc<<" "<<"("<<ic.first<<", "<<ic.second<<")"<<" "<<"("<<ic1.first<<", "<<ic1.second<<")"<<endl;
                    } else {
                        for (int i = 1; i < words.size(); i++) {
                            withoutSymbol.push_back(words[i]);
                        }
                        if (words.size() == 2) {
                            this->sizeOne(withoutSymbol);
                        } else if (words.size() == 3) {
                            if (isdigit(words[2].at(0))) {
                                lc = counter++;
                                ic = t.OPTAB[words[1]];
                                ic1.first = "C";
                                ic1.second = stoi(words[2]);
                                index = t.searchSymbol(words[0]);
                                t.SYMBTAB[index].second = lc;
                                intCode<<lc<<" "<<"("<<ic.first<<", "<<ic.second<<")"<<" "<<"("<<ic1.first<<", "<<ic1.second<<")"<<endl;
                            } else {
                                this->sizeTwo(withoutSymbol);
                            }
                        } else if (words.size() == 4) {
                            this->sizeThree(withoutSymbol);
                        }
                    }
                }
                words.clear();
            }
        } else {
            cout<<"Error opening file"<<endl;
        }
    }
    
    void displaySymbolTable() {
        for (int i = 0; i < t.SYMBTAB.size(); i++) {
            symbTab<<t.SYMBTAB[i].first<<" "<<t.SYMBTAB[i].second<<endl;
        }
    }

    void displayLiteralTable() {
    	for (int i = 0; i < t.LITTAB.size(); i++) {
    		litTab<<t.LITTAB[i].first<<" "<<t.LITTAB[i].second<<endl;
    	}
    }

    void displayPoolTable() {
    	for (int i = 0; i < t.POOLTAB.size(); i++) {
    		poolTab<<t.POOLTAB[i]<<endl;
    	}
    }
};

int main() {
    Assembler a;
    a.passOne();
    a.displaySymbolTable();
    a.displayLiteralTable();
    a.displayPoolTable();
}