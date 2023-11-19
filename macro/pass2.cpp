#include <bits/stdc++.h>
using namespace std;
class macro
{

    vector<tuple<string, int, int, int, int>> MNT;
    vector<vector<string>> PNTAB;
    vector<vector<string>> MDT;
    vector<tuple<string, string>> KPTAB;
    ofstream opFile;
    vector<vector<string>> Tokens;

public:
    macro()
    {
        try
        {

            ifstream ipFile("output.txt", ios::in);
            string buffer, word;
            vector<string> temp;
            while (!ipFile.eof())
            {
                buffer = "";
                temp = {};
                getline(ipFile, buffer);
                stringstream ss(buffer);
                while (ss >> word)
                    temp.push_back(word);
                Tokens.push_back(temp);
            }

            ifstream MNTFile("MNT.txt", ios::in);

            getline(MNTFile, buffer);

            while (!MNTFile.eof())
            {
                int a, b, c, d;
                string n;
                string buffer = "";

                getline(MNTFile, buffer);
                stringstream ss(buffer);
                cout << "BUFFER :" << buffer << endl;

                ss >> n >> a >> b >> c >> d;
                MNT.push_back(make_tuple(n, a, b, c, d));
            }

            ifstream MDTFile("MDT.txt", ios::in);
            getline(MDTFile, buffer);

            while (!MDTFile.eof())
            {
                buffer = "";
                temp = {};
                getline(MDTFile, buffer);
                stringstream ss(buffer);
                while (ss >> word)
                    temp.push_back(word);
                MDT.push_back(temp);
            }

            ifstream PNTABFile("PNTAB.txt", ios::in);
            temp = {};
            getline(PNTABFile, buffer);

            while (!PNTABFile.eof())
            {
                buffer = "";
                temp = {};
                getline(PNTABFile, buffer);
                stringstream ss(buffer);
                while (ss >> word)
                    temp.push_back(word);
                PNTAB.push_back(temp);
            }
            ifstream KPTABFile("KPDTAB.txt", ios::in);
            getline(KPTABFile, buffer);
            while (!KPTABFile.eof())
            {
                string k, v;
                buffer = "";
                getline(KPTABFile, buffer);
                stringstream ss(buffer);
                ss >> k >> v;
                KPTAB.push_back(make_tuple(k, v));
            }
        }
        catch (exception e)
        {
            cerr << "ERROR OCCURED";
        }
    }
    int findMNT(string mName)
    {
        for (int i = 0; i < MNT.size(); i++)
        {
            if (get<0>(MNT[i]) == mName)
            {
                return i;
            }
        }
        return -1;
    }
    int getMacroLength(string mName)
    {
        int index = findMNT(mName);
        int k = get<4>(MNT[index]);
        for (; k < MDT.size(); k++)
        {
            if (MDT[k][0] == "MEND")
            {
                // cout<<"MACRO LEN"<<k+1<<endl;;
                return k + 1 - get<4>(MNT[index]);
            }
        }
        return -1;
    }
    void display()
    {
        cout << "\n------TOKEN------\n";
        for (int i = 0; i < Tokens.size(); i++)
        {
            for (int j = 0; j < Tokens[i].size(); j++)
                cout << Tokens[i][j] << " ";
            cout << endl;
        }
        cout << "\n---------------\n";
        cout << "\n------MDT------\n";
        for (int i = 0; i < MDT.size(); i++)
        {
            for (int j = 0; j < MDT[i].size(); j++)
                cout << MDT[i][j] << " ";
            cout << endl;
        }

        cout << "---------------\n";
        cout << "\n------PNTAB------\n";
        for (int i = 0; i < PNTAB.size(); i++)
        {
            for (int j = 0; j < PNTAB[i].size(); j++)
                cout << PNTAB[i][j] << " ";
            cout << endl;
        }
        cout << "---------------\n";

        cout << "\n------MNT------\n";
        cout << "Name\t#PP\t#KP\t#KDTP\tMDTP\n";
        for (int i = 0; i < MNT.size(); i++)
        {
            cout << get<0>(MNT[i]) << "\t";
            cout << get<1>(MNT[i]) << "\t";
            cout << get<2>(MNT[i]) << "\t";
            cout << get<3>(MNT[i]) << "\t";
            cout << get<4>(MNT[i]) << "\t";
            cout << endl;
        }
        cout << "---------------\n";

        cout << "\n------KPTAB------\n";
        cout << "KW\tVALUE" << endl;
        for (int i = 0; i < KPTAB.size(); i++)
        {
            cout << get<0>(KPTAB[i]) << "\t";
            cout << get<1>(KPTAB[i]) << "\n";
        }
        cout << "---------------\n";
    }
    int getKeywordIndex(string keyword, int PNTAB_index)
    {
        for (int i = 0; i < PNTAB[PNTAB_index].size(); i++)
        {
            if (PNTAB[PNTAB_index][i] == keyword)
                return i;
        }
        return -1;
    }
    void displayAPTAB(vector<string> ap)
    {
        for (auto i : ap)
            cout << i << " ";
    }

    string getValueFromKPTAB(string keyword)
    {
        for (int i = 0; i < KPTAB.size(); i++)
        {
            if (get<0>(KPTAB[i]) == keyword)
                return get<1>(KPTAB[i]);
        }
        return "NOPE";
    }
    void expandMacro(vector<string> curSen)
    {
        int index = findMNT(curSen[0]);
        int numPP = get<1>(MNT[index]);
        int numKP = get<2>(MNT[index]);
        int KDTP = get<3>(MNT[index]);
        int MDTP = get<4>(MNT[index]);

        vector<string> APTAB(numPP + numKP, "NULL");
        for (int i = 1; i < curSen.size(); i++)
        {
            // PP
            if (curSen[i].find("=") == string::npos)
            {
                APTAB[i - 1] = curSen[i];
            }
            else
            { // KP
                string key = curSen[i].substr(0, curSen[i].find("="));
                string val = curSen[i].substr(curSen[i].find("=") + 1);
                // cout<<"--->"<<key<<"-"<<val;
                APTAB[getKeywordIndex(key, index)] = val;
                // cout<<"VAL :"<<getKeywordIndex(key,index)<<"-"<<index;
            }
        }
        for (int i = 0; i < APTAB.size(); i++)
        {
            if (APTAB[i] == "NULL")
                APTAB[i] = getValueFromKPTAB(PNTAB[index][i]);
        }
        displayAPTAB(APTAB);
        for (int i = MDTP; i < MDT.size(); i++)
        {
            for (int j = 0; j < MDT[i].size(); j++)
            {
                if (MDT[i][j] == "MEND")
                {
                    return;
                }
                if (MDT[i][j].substr(0, 1) == "(")
                {
                    int num = stoi(MDT[i][j].substr(MDT[i][j].find(",") + 1, 1));
                    opFile << APTAB[num] << " ";
                }
                else
                {
                    opFile << MDT[i][j] << " ";
                }
            }
            opFile << endl;
        }
    }

    void pass2()
    {
        opFile.open("sourceCode.txt", ios::out);
        for (int i = 0; i < Tokens.size(); i++)
        {
            vector<string> curToken = Tokens[i];
            for (int j = 0; j < Tokens[i].size(); j++)
            {
                if (Tokens[i][j] == "MACRO")
                {
                    i += getMacroLength(Tokens[i + 1][0]) + 1;
                    cout << "DEF" << i << endl;
                    break;
                }
                else if (findMNT(Tokens[i][0]) != -1)
                {
                    // expand
                    cout << "EX" << endl;
                    expandMacro(curToken);
                    break;
                }
                else
                {
                    // write to src
                    opFile << Tokens[i][j] << " ";
                }
            }
            cout << endl;
            opFile << endl;
        }
    }
};

int main()
{
    macro obj;
    obj.display();
    obj.pass2();
    cout << obj.getMacroLength("M1");
    return 0;
}