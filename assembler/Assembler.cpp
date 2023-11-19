#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class AssemblerP1
{
    map<string, pair<string, int>> memo;
    map<string, int> reg;
    map<string, int> concode;
    int lc;
    int address;

    // symbol and literal ans pool tabel:
    vector<pair<string, int>> sym;
    vector<pair<string, int>> lit;
    vector<int> pol;

    // File creation for new output :
    fstream fin, icout, stout, ltout, pout;

public:
    AssemblerP1()
    {
        // memonics:
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
        // registers:
        reg["AREG"] = 1;
        reg["BREG"] = 2;
        reg["CREG"] = 3;
        reg["DREG"] = 4;
        // concodes:
        concode["LT"] = 1;
        concode["LE"] = 2;
        concode["EQ"] = 3;
        concode["GT"] = 4;
        concode["GE"] = 5;
        concode["ANY"] = 6;

        // other:
        address = 0;
        lc = 0;
    }

    void takeSourceCodeInput()
    {
        fin.open("input2.txt", ios::in);
        if (!fin)
        {
            cout << "Error in Opening File" << endl;
        }
        else
        {
            cout << "Input Taken  Successfully" << endl;
        }
    }
    void insertInSymbolandLiteralTableandPoolTable()
    {
        stout.open("symTable.txt", ios::out);
        stout << "----------SYMBOL TABLE---------\n"
              << endl;
        ltout.open("litTable.txt", ios::out);
        ltout << "----------LITERAL TABLE---------\n"
              << endl;
        pout.open("poolTable.txt", ios::out);
        pout << "----------POOL TABLE---------\n"
             << endl;
        for (int i = 0; i < sym.size(); i++)
        {
            stout << sym[i].first << "," << sym[i].second << endl;
        }
        for (int i = 0; i < lit.size(); i++)
        {
            ltout << lit[i].first << "," << lit[i].second << endl;
        }
        for (int i = 0; i < pol.size(); i++)
        {
            pout << pol[i] << endl;
        }
    }

    vector<string> breakInWords(string buffer)
    {
        vector<string> words;
        string word = "";
        buffer += " ";
        for (auto x : buffer)
        {

            if (x == ' ')
            {
                words.push_back(word);
                word = "";
            }
            else
            {
                word += x;
            }
        }
        return words;
    }
    int found(vector<pair<string, int>> s, string key)
    {
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i].first == key)
            {
                return i + 1;
            }
        }
        return -1;
    }
    void pass1Assembler()
    {
        bool gotstart = false;
        bool gotltorg = false;
        int litcount = 0;
        takeSourceCodeInput();
        // intermediate code file creation
        icout.open("InterMCode.txt", ios::out);

        while (!fin.eof())
        {
            // incrementing LC;
            lc++;

            // print LC when Address is Assigned:
            // if (gotstart)
                // icout << lc << "\t";
            // taking each line from the source code.
            string buffer;
            getline(fin, buffer);

            vector<string> words = breakInWords(buffer); // breaking in nof of words in each line

            if (words.size() == 4)
            {
                // if there are 4 words then order is lable-memonics-op1-op2
                string lable, memoni, op1, op2;
                lable = words[0];
                memoni = words[1];
                op1 = words[2];
                op2 = words[3];
                if (found(sym, lable) != -1)
                {
                    sym[found(sym, lable) - 1].second = lc;
                    //					      icout<<"(S,"<<found(sym,lable)<<")";
                }

                else if (found(sym, lable) == -1)
                {
                    sym.push_back({lable, lc});
                    //						 icout<<"(S,"<<found(sym,lable)<<")";
                }

                icout << "(" << memo[memoni].first << "," << memo[memoni].second << ")\t";
                icout << "(" << reg[op1] << ")\t";

                if (found(sym, op2) != -1)
                {
                    // sym[found(sym, op2) - 1].second = lc;
                    icout <<"(S,"<< found(sym, op2) << ")\t" << endl;
                }
                else if (found(sym, op2) == -1)
                {    
                       if (op2[0] == '=')
                        {
                            lit.push_back({op2, -1});
                            icout << "(L," << found(lit, op2) << ")" << endl;
                            litcount++;
                            if (gotltorg == true)
                            {
                                pol.push_back(found(lit, op2));
                                gotltorg = false;
                            }
                        }
                        else
                        {
                            sym.push_back({op2, -1});
                            icout << "(S," << found(sym, op2) << ")" << endl;
                        }
                  
                }
            }
            else if (words.size() == 3)
            { // if size is 3 then we have two conditions that if EQU is Memonic or not .
                // handeled seperately
                string memoni, op1, op2;
                memoni = words[0];
                op1 = words[1];
                op2 = words[2];

                if (memo.find(memoni) != memo.end() && memoni != "EQU")
                { // Memonics is present ant not EQU directive.
                    icout << "(" << memo[memoni].first << "," << memo[memoni].second << ")\t";

                    if (memoni == "BC")
                    {
                        icout << "(" << concode[op1] << ")\t";
                    }
                    else
                    {
                        icout << "(" << reg[op1] << ")\t";
                    }
                    if (found(sym, op2) != -1)
                    {
                        // sym[found(sym, op2) - 1].second = lc;
                        icout << "(S," << found(sym, op2) << ")" << endl;
                    }
                    else if (found(sym, op2) == -1)
                    {
                        if (op2[0] == '=')
                        {
                            lit.push_back({op2, -1});
                            icout << "(L," << found(lit, op2) << ")" << endl;
                            litcount++;
                            if (gotltorg == true)
                            {
                                pol.push_back(found(lit, op2));
                                gotltorg = false;
                            }
                        }
                        else
                        {
                            sym.push_back({op2, -1});
                            icout << "(S," << found(sym, op2) << ")" << endl;
                        }
                    }
                }

                else
                { // the format is different and EQU is handeled here .
                    string lable, mem, cons;
                    lable = memoni;
                    mem = op1;
                    cons = op2;
                    if (mem == "EQU")
                    {

                        sym.push_back({lable, sym[found(sym, cons) - 1].second});
                        icout << "(" << memo[mem].first << "," << memo[mem].second << ")\t";
                        icout << "(S," << found(sym, cons) << ")" << endl;
                    }
                    else
                    {
                        if (found(sym, lable) != -1)
                        {
                            sym[found(sym, lable) - 1].second = lc;
                            //  icout<<"(S,"<<found(sym,lable)<<")";
                        }

                        else if (found(sym, lable) == -1)
                        {
                            sym.push_back({lable, lc});
                            // icout<<"(S,"<<found(sym,lable)<<")";
                        }

                        icout << "(" << memo[mem].first << "," << memo[mem].second << ")\t(C," << cons << ")" << endl;
                    }
                }
            }
            else if (words.size() == 2)
            {
                string memoni, lable;
                memoni = words[0];
                lable = words[1];
                if (memoni == "START")
                {
                    gotstart = true;
                    address = stoi(lable);
                    lc = address - 1;
                    icout << "(" << memo[memoni].first << "," << memo[memoni].second << ")\t(C," << address << ")" << endl;
                }
                else if (memoni == "ORIGIN")
                {
                    address = sym[found(sym, lable) - 1].second;
                    lc = address - 1;
                    icout << "(" << memo[memoni].first << "," << memo[memoni].second << ")\t(S," << found(sym, lable) << ")" << endl;
                }
                else
                {
                    //            	sym.push_back({lable,-1});
                    icout << "(" << memo[memoni].first << "," << memo[memoni].second << ")\t(S,";
                    if (found(sym, lable) != -1)
                    {
                        icout << found(sym, lable) << ")" << endl;
                    }
                    else if (found(sym, lable) == -1)
                    {
                        sym.push_back({lable, -1});
                        icout << found(sym, lable) << ")" << endl;
                    }
                }
            }
            else if (words.size() == 1)
            {
                string memoni;
                memoni = words[0];
                if (memoni == "LTORG")
                {
                    icout << "(" << memo[memoni].first << "," << memo[memoni].second << ")" << endl;

                    if (!gotltorg)
                    {
                        gotltorg = true;
                    }
                    if (pol.empty() && litcount != 0)
                    {
                        pol.push_back(1);
                        int i = 0;
                        while (i < litcount)
                        {
                            
                            lit[i++].second = lc++;
                        }
                        lc--;
                        litcount = 0;
                    }
                    else
                    {
                        int i = lit.size() - litcount;
                        while (i <= lit.size())
                        {
                            
                            lit[i++].second = lc++;
                        }
                        lc -= 2;
                        litcount = 0;
                    }
                }
                else if ( memoni == "STOP")
                {
                    icout << "(" << memo[memoni].first << "," << memo[memoni].second << ")" << endl;
                }
                else if(memoni == "END"){

                    if (pol.empty() && litcount != 0)
                    {
                        pol.push_back(1);
                        int i = 0;
                        while (i < litcount)
                        {
                            cout << i << endl;
                            lit[i++].second = lc++;
                        }
                        lc--;
                        litcount = 0;
                    }
                    else
                    {
                        int i = lit.size() - litcount;
                        while (i <= lit.size())
                        {
                            cout << i << endl;
                            lit[i++].second = lc++;
                        }
                        lc -= 2;
                        litcount = 0;
                    }
                    icout << "(" << memo[memoni].first << "," << memo[memoni].second << ")" << endl;

                    cout << "Check the Output Files for the Answer." << endl;
                }
            }
            else if (words.size() != 0)
            {
                cout << "Syntax Error in source code " << endl;
            }
        }
        insertInSymbolandLiteralTableandPoolTable();
        fin.close();
    }
    

    vector<string> breakforBrackets(string buffer){
        vector<string> words;
        string word = "";
        buffer += " ";
        int i=0;
        while(i<buffer.length())
        {  
            if(buffer[i]='('){
                i++;
                while(buffer[i]!=')'){
                    word+=buffer[i++];
                }
                words.push_back(word);
                word="";
            }
            i++;
            
        }
        return words;
    }

    vector<string> breakComm(string buffer){
        vector<string>words;
        string word="";
        buffer+=",";
        int i=0;
        while(i<buffer.length()){
            if(buffer[i]=','){
                words.push_back(word);
                word="";
            }
            else{
                word+=buffer[i];
            }
            i++;
        }
    }
    ~AssemblerP1()
    {
        stout.close();
        ltout.close();
        pout.close();
        icout.close();
    }
};

int main()
{
    AssemblerP1 a1;
      
    a1.pass1Assembler();

    return 0;
}