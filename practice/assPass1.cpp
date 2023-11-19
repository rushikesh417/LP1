#include<bits/stdc++.h>
using namespace std;

class Instruction{
    
    public:
    string label, mnemonic, op1, op2;

    Instruction(string label="", string mnemonic="", string op1="", string op2=""){
        this->label = label;
        this->mnemonic = mnemonic;
        this->op1 = op1;
        this->op2 = op2;
    }

    bool hasLabel(){
        if(this->label!=""){
            return true;
        }
        return false;
    }
    
    bool hasOp1(){
        if(this->op1!=""){
            return true;
        }
        return false;
    }

    bool hasOp2(){
        if(this->op2!=""){
            return true;
        }
        return false;
    }

};


class OpcodeTable{

    public:

    map<string, pair<string, int>> lookup = {
        {"STOP", {"IS", 0}},
        {"ADD", {"IS", 1}},
        {"SUB", {"IS", 2}},
        {"MULT", {"IS", 3}},
        {"MOVER", {"IS", 4}},
        {"MOVEM", {"IS", 5}},
        {"COMP", {"IS", 6}},
        {"BC", {"IS", 7}},
        {"DIV", {"IS", 8}},
        {"READ", {"IS", 9}},
        {"PRINT", {"IS", 10}},

        {"START", {"AD", 1}},
        {"END", {"AD", 2}},
        {"ORIGIN", {"AD", 3}},
        {"EQU", {"AD", 4}},
        {"LTORG", {"AD", 5}},

        {"DC", {"DL", 1}},
        {"DS", {"DL", 2}},

        {"AREG", {"RG", 1}},
        {"BREG", {"RG", 2}},
        {"CREG", {"RG", 3}},
        {"DREG", {"RG", 4}},

        {"LT", {"CC", 1}},
        {"LT", {"CC", 2}},
        {"EQ", {"CC", 3}},
        {"GT", {"CC", 4}},
        {"GT", {"CC", 5}},
        {"GE", {"CC", 6}}
    };

    // RETURNS THE CLASS OF GIVEN MNEMONIC
    string get_class(string mnemonic){
        return lookup[mnemonic].first;
    }

    // RETURN THE CODE OF GIVEN MNEMONIC
    int get_code(string mnemonic){
        return lookup[mnemonic].second;
    }

    // CHECKS IF GIVEN IS MNEMONIC
    bool is_mnemonic(string name){
        if(lookup.find(name) != lookup.end()){
            return true;
        }
        return false;
    }

};

bool inSymTab(string s,vector<pair<string, int>>& symTab){
    for(auto i: symTab){
            if(i.first == s){
                return true;
            }
        }
        return false;
}

int main(){

    ifstream fin;
    fin.open("input.txt" ,ios::in);

    string line ,word;

    vector<Instruction>vec;

    while(getline(fin ,line)){
        stringstream ss(line);
        vector<string>v;
        while(ss>>word){
            
            v.push_back(word);
        }

        Instruction obj;
        OpcodeTable ob;

        if(ob.is_mnemonic(v[0])){
            obj.mnemonic=v[0];
            if(v.size()>1){
                obj.op1 = v[1];
                if(v.size()==3){
                    obj.op2 = v[2];
                }
            }
        }
        else if(ob.is_mnemonic(v[1])){
            obj.label=v[0];
            obj.mnemonic=v[1];
            obj.op1=v[2];
            if(v.size()==4){
                obj.op2 = v[3];
            }
        }

        

        vec.push_back(obj);
        v.clear();
    }

    for(auto it :vec){
        cout<<it.label<< " "<<it.mnemonic<< " "<<it.op1<<" "<<it.op2<<endl;
    }
    

    vector<pair<int, string>> iCode;
    vector<pair<string, int>> symTab;
    vector<pair<string, int>> litTab;
    vector<int> poolTab;

    int lc, stPtr=0, ltPtr=0, ptPtr=0;

    OpcodeTable op;

    for(auto inst : vec){

        string ic="";

        // treating the lables
        if(inst.hasLabel()){
            if(!inSymTab(inst.label ,symTab)){
                symTab.push_back({inst.label, lc});
                stPtr++;
            }
            else{
                for(int s=0; s<symTab.size(); s++){
                        if(symTab[s].first == inst.label){
                            symTab[s].second = lc;
                        }
                    }
            }
        }


        //treating the memo and opcodes

        string mnemonic = inst.mnemonic;

        if(op.get_class(mnemonic) == "IS"){
            ic+= "(" + op.get_class(mnemonic) + ", " + to_string(op.get_code(mnemonic))+") ";
            
            if(inst.hasOp1()){
                    string op1 = inst.op1;
                    if(op.get_class(op1)=="RG"){
                        ic += "(" + op.get_class(op1) + "," + to_string(op.get_code(op1)) + ") ";
                    }
                    else if(op.get_class(op1)=="CC"){
                        ic += "(" + op.get_class(op1) + "," + to_string(op.get_code(op1)) + ") ";
                    }else{
                        symTab.push_back({op1, -1});
                        ic += "(S," + to_string(stPtr) + ") ";
                        stPtr++;
                    }
            }
            if(inst.hasOp2()){
                    string op2 = inst.op2;
                    // CHECK IF LITERAL
                    if(op2[0]=='='){
                        litTab.push_back({op2, -1});
                        ic += "(L," + to_string(ltPtr) + ") ";
                        ltPtr++;
                    }
                    //CHECK IF SYMBOL
                    else{
                        if(!inSymTab(op2 ,symTab)){
                            symTab.push_back({op2, -1});
                            ic += "(S," + to_string(stPtr) + ") ";
                            stPtr++;
                        }
                    }
            }
            lc++;
                
        }
        else if(mnemonic=="START"){
            ic+= "(" + op.get_class(inst.mnemonic) + ", " + to_string(op.get_code(inst.mnemonic))+") ";
            
                if(inst.hasOp1()){
                    lc = stoi(inst.op1);
                    ic += "(C," + inst.op1 + ") ";
                }
                else{
                    lc = 0;
                }
            lc++;

        }
        else if(mnemonic=="END"){
            ic+= "(" + op.get_class(inst.mnemonic) + ", " + to_string(op.get_code(inst.mnemonic))+") ";
        }
        else if(mnemonic=="LTORG"){

            for(int i=ptPtr ;i<litTab.size();i++){
                litTab[i].second=lc;
                lc++;

                string lit = litTab[i].first;
                    lit = lit.substr(2, lit.length()-3);
                    ic += "(DL,01) (C," + lit + ") ";

                    // cout<<ic<<endl;
                    iCode.push_back({lc,ic});
                    ic="";
                ptPtr++;
            }

        }
        else if(mnemonic=="ORIGIN"){
            ic+="(" + op.get_class(inst.mnemonic) + ", " + to_string(op.get_code(inst.mnemonic))+") ";

            
            int index=0;
            for(int i=0;i<inst.op1.size();i++){
                if(inst.op1[i]=='+' || inst.op1[i]=='-'){
                    index=i;
                    break;
                }
            }
            string before = inst.op1.substr(0 ,index);
            string after = inst.op1.substr(index);
            cout<<before<<endl;
            cout<<after<<endl;
            // int num = stoi(after);

            int newLc = 0;
            int ind=0;
            int cnt=1;
                for(auto i :symTab){
                    if(i.first==before){
                        newLc = i.second;
                        ind = cnt;
                        break;
                    }
                    cnt++;
                }

            ic+= " (S, "+to_string(ind)+")"+after;
            before="";
            after="";

        }
        else if(mnemonic=="EQU"){

        }
        else if(mnemonic=="DS"){
            ic += "(" + op.get_class("DS") + "," + to_string(op.get_code("DS")) + ") ";

                // FIND THE LABEL IN SYMBTAB
                for(int s=0; s<symTab.size(); s++){
                    if(symTab[s].first==inst.label){
                        symTab[s].second = lc;
                    }
                }

                // IC FOR OP1
                ic += "(C,"+inst.op1+") ";

                lc++;
        }
        else if(mnemonic=="DC"){
            // ADD IC FOR MNEMONIC
                ic += "(" + op.get_class("DC") + "," + to_string(op.get_code("DC")) + ") ";

                // FIND THE LABEL IN SYMBTAB
                for(int s=0; s<symTab.size(); s++){
                    if(symTab[s].first==inst.label){
                        symTab[s].second = lc;
                    }
                }

                // IC FOR OP1
                ic += "(C,"+inst.op1.substr(1,inst.op1.size()-2)+") ";

            lc++;
        }

        iCode.push_back({lc ,ic});
    }

    cout<<"-------------------"<<endl;
    for(auto it:iCode){
        cout<<it.first<<" "<<it.second<<endl;
    }
    cout<<"-------------------"<<endl;
    for(auto it:symTab){
        cout<<it.first<<" "<<it.second<<endl;
    }
    cout<<"-------------------"<<endl;
    for(auto it:litTab){
        cout<<it.first<<" "<<it.second<<endl;
    }

    return 0;
}