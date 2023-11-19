#include <iostream>
#include <bits/stdc++.h>
#define long long ll
using namespace std;
class machineOpcodeTable
{
    map<string, pair<string, int>> memo;
    map<string, pair<string, int>> reg;
    map<string, pair<string, int>> concode;

public:
    machineOpcodeTable()
    {
        memo["STOP"] = {"IS", 00};
        memo["ADD"] = {"IS", 01};
        memo["SUB"] = {"IS", 02};
        memo["MULT"] = {"IS", 03};
        memo["MOVER"] = {"IS", 04};
        memo["MOVEM"] = {"IS", 05};
        memo["COMP"] = {"IS", 06};
        memo["BC"] = {"IS", 07};
        memo["DIV"] = {"IS", 8};
        memo["READ"] = {"IS", 9};
        memo["PRINT"] = {"IS", 10};
        memo["START"] = {"AD", 01};
        memo["END"] = {"AD", 02};
        memo["ORIGIN"] = {"AD", 03};
        memo["EQU"] = {"AD", 04};
        memo["LTORG"] = {"AD", 05};
        memo["DS"] = {"DL", 02};
        memo["DC"] = {"DL", 01};

        reg["AREG"] = {"RG", 1};
        reg["BREG"] = {"RG", 2};
        reg["CREG"] = {"RG", 3};
        reg["DREG"] = {"RG", 4};

        concode["LT"] = {"CC", 1};
        concode["LE"] = {"CC", 2};
        concode["EQ"] = {"CC", 3};
        concode["GT"] = {"CC", 4};
        concode["GE"] = {"CC", 5};
        concode["NE"] = {"CC", 6};
        concode["ANY"] = {"CC", 7};
    }
    pair<string, int> getmemo(string opcode)
    {
        auto itr = memo.find(opcode);
        if (itr != memo.end())
        {
            return memo[opcode];
        }
        itr = reg.find(opcode);
        if (itr != reg.end())
        {
            return reg[opcode];
        }
        itr = concode.find(opcode);
        if (itr != concode.end())
        {
            return concode[opcode];
        }

        if (opcode[0] == '=')
        {
            return {"L", -1};
        }
        return {"S", -1};
    }
};
class symbolTable
{
    vector<pair<string, int>> symtab;
    friend class assembler;
    int symcount = 0;

public:
    symbolTable()
    {
    }
    bool present(string sym)
    {
        for (int i = 0; i < symtab.size(); i++)
        {
            if (symtab[i].first == sym)
            {
                return true;
            }
        }
        return false;
    }
    int getindex(string sym)
    {
        for (int i = 0; i < symtab.size(); i++)
        {
            if (symtab[i].first == sym)
            {
                return i + 1;
            }
        }
        return -1;
    }
    void handlesym(string sym, int curlc, bool flag)
    {
        // flag is for forward defination : means if the symbol is not 1st then dont add any address value
        if (flag)
        {
            if (!present(sym))
            {
                symtab.push_back({sym, curlc}); // forward defination
                symcount++;
            }
            else
            {
                symtab[getindex(sym) - 1].second = curlc; // when dl stamement is used
            }
        }
        else
        {
            if (!present(sym))
            {
                symtab.push_back({sym, 999});
                symcount++;
            }
        }
    }
    int getaddress(string sym)
    {
        return symtab[getindex(sym) - 1].second;
    }
    void display()
    {
        cout << "********Symbol" << endl;
        for (int i = 0; i < symtab.size(); i++)
        {
            cout << symtab[i].first << " -> " << symtab[i].second << endl;
        }
        cout << "*********" << endl;
    }
};

class literalTabel
{
public:
    vector<pair<string, int>> littab;
    vector<string> lpool;
    int litcount = 0;
    friend class assembler;
    bool present(string lit)
    {
        for (int i = 0; i < littab.size(); i++)
        {
            if (littab[i].first == lit)
            {
                return true;
            }
        }
        return false;
    }
    int getindex(string lit)
    {
        for (int i = 0; i < lpool.size(); i++)
        {
            if (lpool[i] == lit)
            {
                return i + 1;
            }
        }
        return -1;
    }
    void updateaddress(string lit, int add)
    {
        littab.push_back({lit, add});
    }
    int compute(int &curlc)
    {
        for (int i = 0; i < lpool.size(); i++)
        {
            updateaddress(lpool[i], curlc);
            curlc++;
        }
        lpool = {};
        curlc--;
        return curlc;
    }
    void disp()
    {
        cout << "********Literea" << endl;
        for (int i = 0; i < littab.size(); i++)
        {
            cout << littab[i].first << " -> " << littab[i].second << endl;
        }
        cout << "*********" << endl;
    }
};
class asembler
{
    machineOpcodeTable machine;
    symbolTable symt;
    literalTabel lit;
    vector<int> pooltab;
    ifstream inp;
    ofstream outp;

public:
    void pass1()
    {
        int lc = 0;
        bool gotstart = false;
        int address = 0;
        outp.open("intermcode.txt", ios::out);
        inp.open("inp.txt", ios::in);
        if (inp)
        {
            cout << "Error in input file" << endl;
        }
        else
        {
            cout << "Input taken succ " << endl;
        }
        while (!inp.eof())
        {
            lc++;
            if (gotstart)
            {
                outp << lc << "\t";
            } // only after start derective ;

            string buffer;
            getline(inp, buffer);
            stringstream ss(buffer);
            string words;
            vector<string> tokens;
            while (ss >> words)
            {
                tokens.push_back(words);
            } // we have now the array fo the breaked wrods in the line .

            // computaion:
            if (tokens.size() == 4)
            {
                // if 4 words then fixed order: lable-memonics-reg-opreand(sym/lit)
                // take it from token
                string lable, memonics, reg, op;
                lable = tokens[0];
                memonics = tokens[1];
                reg = tokens[2];
                op = tokens[3];
                // check the lable is already in symbol table or not
                symt.handlesym(lable, lc, true); // true beacuse we have to assing address nnot 999
                // no output for forward defination
                outp << "\t(" << machine.getmemo(memonics).first << " " << machine.getmemo(memonics).second << ")\t";
                outp << "(" << machine.getmemo(reg).first << " " << machine.getmemo(reg).second << ")\t";
                // the op is symbol but no forward defination tuhs flag will be false;
                // but check for literal or not
                if (op[0] == '=')
                {
                    lit.lpool.push_back(op);
                    lit.litcount++;
                    outp << "(" << machine.getmemo(op).first << " " << lit.getindex(op) << ")\n";
                }
                else
                {
                    symt.handlesym(op, lc, false);
                    outp << "(" << machine.getmemo(op).first << " " << symt.getindex(op) << ")\n";
                }
            }
            else if (tokens.size() == 3)
            {
                // now 3 words two case normal and equ seperatly handled.
                string fst, snd, trd;
                fst = tokens[0];
                snd = tokens[1];
                trd = tokens[2];
                if (snd == "DS" || snd == "DC")
                {
                    // dclerative  lable ds/dc
                    symt.handlesym(fst, lc, true); // here assing the address
                    outp << "\t(" << machine.getmemo(snd).first << " " << machine.getmemo(snd).second << ")";
                    outp << "(C " << trd << ")" << endl;
                    if (trd == "DS")
                    {
                        lc += stoi(trd) - 1; // increment by length
                    }
                }
                else if (machine.getmemo(fst).first == "IS")
                {
                    // IS  statments so memo-reg-operand(sym/lit);
                    outp << "\t(" << machine.getmemo(fst).first << " " << machine.getmemo(fst).second << ")\t";
                    outp << "(" << machine.getmemo(snd).first << " " << machine.getmemo(snd).second << ")\t";
                    // check lit and no forwadr defination
                    if (trd[0] == '=')
                    {
                        lit.lpool.push_back(trd);
                        lit.litcount++;
                        outp << "(" << machine.getmemo(trd).first << " " << lit.getindex(trd) << ")\n";
                    }
                    else
                    {
                        symt.handlesym(trd, lc, false);
                        outp << "(" << machine.getmemo(trd).first << " " << symt.getindex(trd) << ")\n";
                    }
                }
                else if (snd == "EQU")
                {
                    // equ case
                    symt.handlesym(fst, symt.getaddress(trd), true); // direct adrresing but not lc
                    outp << "\t(" << machine.getmemo(snd).first << " " << machine.getmemo(snd).second << ")\t";

                    outp << "(" << machine.getmemo(trd).first << " " << symt.getindex(trd) << ")\n";
                }
            }
            else if (tokens.size() == 2)
            { // here start,read/print and origin is handled
                string memo, snd;
                memo = tokens[0];
                snd = tokens[1];
                if (memo == "START")
                {
                    gotstart = true;
                    address = stoi(snd);
                    lc = address - 1;
                    outp << "\t(" << machine.getmemo(memo).first << " " << machine.getmemo(memo).second << ")\t";
                    outp << "(C " << address << ")\n";
                }
                else if (memo == "ORIGIN")
                {
                    string newsnd;
                    if (snd.find("+") != string::npos)
                    {
                        int offset = stoi(snd.substr(snd.find("+") + 1, snd.length()));
                        newsnd = snd.substr(0, snd.find("+"));
                        address = symt.getaddress(newsnd);
                        lc = address + offset - 1;
                    }
                    else if (snd.find("-") != string::npos)
                    {
                        int offset = stoi(snd.substr(snd.find("-") + 1, snd.length()));
                        newsnd = snd.substr(0, snd.find("-"));
                        address = symt.getaddress(newsnd);
                        lc = address - offset - 1;
                    }
                    else
                    {
                        address = symt.getaddress(snd);
                        lc = address + -1;
                        newsnd = snd;
                    }
                    outp << "\t(" << machine.getmemo(memo).first << " " << machine.getmemo(memo).second << ")\t";
                    outp << "(" << machine.getmemo(newsnd).first << " " << symt.getindex(newsnd) << ")\n";
                }
                else
                {
                    // here all ther where memo-symbol pattern
                    outp << "\t(" << machine.getmemo(memo).first << " " << machine.getmemo(memo).second << ")\t";
                    symt.handlesym(snd, lc, false); // no forward declaration;
                    outp << "(" << machine.getmemo(snd).first << " " << symt.getindex(snd) << ")\n";
                }
            }
            else if (tokens.size() == 1)
            {
                // here ltorg and stop
                string memo = tokens[0];
                outp << "\t(" << machine.getmemo(memo).first << " " << machine.getmemo(memo).second << ")\n";
                if (memo == "LTORG" || memo == "END")
                {
                    int poolindex = lit.littab.size() + 1;
                    pooltab.push_back(poolindex);
                    lit.compute(lc);
                    if (memo == "END")
                    {
                        cout << "Done Generation of intermediate code" << endl;
                    }
                }
            }
            else
            {
                cout << "Erro in the input file at line " << endl;
            }
        }

        symt.display();
        lit.disp();
        outp.close();
        inp.close();
        cout << "====pool" << endl;
        for (int i = 0; i < pooltab.size(); i++)
        {
            cout << pooltab[i] << endl;
        }
        cout << "====" << endl;
    }
};

int main()
{
    asembler a1;
    a1.pass1();

    return 0;
}