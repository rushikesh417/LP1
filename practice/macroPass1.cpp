#include<bits/stdc++.h>
using namespace std;

int main(){

    ifstream fin;
    fin.open("source.txt" , ios::in);
    ofstream mntout,kpdtout,pntout,mdtout,fout;
    mntout.open("MNT.txt");
    mdtout.open("MDT.txt");
    kpdtout.open("KPDTAB.txt"); 
    pntout.open("PNTAB.txt");
    fout.open("output.txt");

    vector<array<string,5>>mnt;
    vector<pair<string,string>>kpd;
    map<string ,int>pnt;

    string line ,word;

    bool macro=false ,start = false;

    int mdtptr = 1;

    while(getline(fin ,line)){

        stringstream ss(line);
        ss >> word;

        if(word == "MACRO"){
            macro=true;
            continue;
        }
        if(macro){
            array<string,5>temp={""};
            temp[0]=word;
            temp[3]=to_string(mdtptr);

            int pp = 0, kp = 0;
            while(ss >> word){
                word = word.substr(1);
                if(word[word.size()-1]== ','){
                    word = word.substr(0 , word.size() -1 );
                }

                int kpidx = -1;
                for (int i = 0; i < word.size(); i++)
                {
                    if (word[i] == '=')
                    {
                        kpidx = i;
                        break;
                    }
                }

                if(kpidx != -1){
                    kp++;
                    pair<string,string>pr({word.substr(0 , kpidx), word.substr(kpidx+1)});
                    kpd.push_back(pr);

                    pnt[pr.first] = pnt.size() + 1;
                    pntout << pnt[pr.first] << "\t" << pr.first << "\n";
                }
                else{
                    pp++;

                    pnt[word]=pnt.size()+1;
                    pntout << pnt[word] << " "<< word<<endl;

                }

            }
            temp[1] = to_string(pp);
            temp[2] = to_string(kp);
            if (kp > 0)
                temp[4] = to_string(kpd.size() - kp + 1);

            mnt.push_back(temp);
            macro=false;
            continue;
        }
        if(word == "START"){
            start=true;
            continue;
        }

        if(start){

            if (word != "END")
            {
                fout << word << " ";
                while (ss >> word)
                {
                    fout << word << " ";
                }
                fout << "\n";
            }
            continue;
        }

        if(word=="MEND"){
            mdtout << word;
            pnt.clear();
        }
        else{

            mdtout << word << " ";

            while (ss >> word)
            {

                if (word[0] == '&')
                {
                    word = word.substr(1);
                    if (word[word.size() - 1] == ',')
                        word = word.substr(0, word.size() - 1);

                    mdtout << "(P," << pnt[word] << ") ";
                }
                else if(word[0]=='='){
                    mdtout  << word ;
                }
            }
            mdtout << "\n";

        }

        

        mdtptr++;

    }

    for (int i = 0; i < mnt.size(); i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << mnt[i][j] << " ";
            mntout << mnt[i][j] << " ";
        }
        cout << "\n";
        mntout << "\n";
    }

    for (int i = 0; i < kpd.size(); i++)
    {
        kpdtout << kpd[i].first << " " << kpd[i].second << "\n";
    }

    return 0;
}