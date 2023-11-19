#include<bits/stdc++.h>
using namespace std;

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

class Tokenizer{
    
    public:

    // RETURNS ALL TOKENIZED LINES OF INPUT FILE
    vector<Instruction> TokenizeAll(){

        vector<Instruction> tokenizedLines;

        // OPEN FILE
        ifstream inputFile;
        inputFile.open("tc1.txt", ios::out);

        // EXTRACT EACH LINE AND TOKENIZE IT
        string line;
        while(getline(inputFile, line)){
            Instruction i = TokenizeSingle(line);
            tokenizedLines.push_back(i);
        }

        // CLOSE FILE
        inputFile.close();

        return tokenizedLines;

    }

    // RETURN SINGLE TOKENIZED LINE
    Instruction TokenizeSingle(string line){
        
        OpcodeTable opt;
        Instruction inst;
        vector<string> tokens;

        string token;
        stringstream ss(line);
        while(getline(ss, token, ' ')){
            tokens.push_back(token);
        }

        // LABEL IS NOT PRESENT
        if(opt.is_mnemonic(tokens[0])){
            inst.mnemonic = tokens[0];
            if(tokens.size()>1){
                inst.op1 = tokens[1];
                if(tokens.size()==3){
                    inst.op2 = tokens[2];
                }
            }
        }

        // LABEL IS PRESENT
        else if(opt.is_mnemonic(tokens[1])){
            inst.label = tokens[0];
            inst.mnemonic = tokens[1];
            inst.op1 = tokens[2];
            if(tokens.size()==4){
                inst.op2 = tokens[3];
            }
        }

        return inst;

    }
    
};

class Pass1{

    public:

    // 1] TOKENIZE ALL THE INSTRUCTIONS
    Tokenizer tknz;
    vector<Instruction> instSet;

    // 2] DATASTRUCTURES REQUIRED
    vector<pair<int, string>> iCode;
    vector<pair<string, int>> symTab;
    vector<pair<string, int>> litTab;
    vector<int> poolTab;

    // 3] TABLE POINTERS
    int lc, stPtr, ltPtr, ptPtr;

    OpcodeTable opt;

    Pass1(){
        this->instSet = tknz.TokenizeAll();
        this->stPtr = 0;
        this->ltPtr = 0;
        this->ptPtr = 0;
    }

    // CHECK IF LABEL PRESENT IN SYMTAB
    bool isInSymTab(string lbl){
        for(auto i: this->symTab){
            if(i.first == lbl){
                return true;
            }
        }
        return false;
    }

    // PRINT SYMBTAB
    void printSymTab(){
        for(int i=0; i<this->symTab.size(); i++){
            cout<<i<<" "<<this->symTab[i].first<<" "<<this->symTab[i].second<<endl;
        }
    }

    // PRINT LITTABLE
    void printLitTab(){
        for(int i=0; i<litTab.size(); i++){
            cout<<i<<" "<<this->litTab[i].first<<" "<<this->litTab[i].second<<endl;
        }
    }

    // PRINT ICODE
    void printICode(){
        for(auto i: iCode){
            cout<<i.first<<" "<<i.second<<endl;
        }
    }

    // PERFORM PASS1
    void performPass1(){

        for(auto inst: this->instSet){

            string ic = "";

            // PROCESS LABELS
            if(inst.hasLabel()){
                if(!isInSymTab(inst.label)){
                    symTab.push_back({inst.label, lc});
                    stPtr++;
                }else{
                    for(int s=0; s<symTab.size(); s++){
                        if(symTab[s].first == inst.label){
                            symTab[s].second = lc;
                        }
                    }
                }
            }

            // PROCESS MNEMONIC AND OPCODES
            string mnemonic = inst.mnemonic;

            if(opt.get_class(mnemonic) == "IS"){
                
                // ADD IC FOR MNEMONIC
                ic += "(" + opt.get_class(mnemonic) + "," + to_string(opt.get_code(mnemonic)) + ") ";
                
                // ADD IC FOR OP1
                if(inst.hasOp1()){
                    string op1 = inst.op1;
                    if(opt.get_class(op1)=="RG"){
                        ic += "(" + opt.get_class(op1) + "," + to_string(opt.get_code(op1)) + ") ";
                    }
                    else if(opt.get_class(op1)=="CC"){
                        ic += "(" + opt.get_class(op1) + "," + to_string(opt.get_code(op1)) + ") ";
                    }else{
                        if(!isInSymTab(op1)){
                            symTab.push_back({op1, -1});
                            ic += "(S," + to_string(stPtr) + ") ";
                            stPtr++;
                        }else{
                            // ITERATE IN SYMTAB AND GET INDEX OF THE SYMB, AND GENERATE IC USING THAT INDX
                        }
                    }
                }

                // ADD IC FOR OP2
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
                        if(!isInSymTab(op2)){
                            symTab.push_back({op2, -1});
                            ic += "(S," + to_string(stPtr) + ") ";
                            stPtr++;
                        }else{
                            // ITERATE IN SYMTAB AND GET INDEX OF THE SYMB, AND GENERATE IC USING THAT INDX
                        }
                    }
                }
                
                // INCREMENT LC
                this->lc++;

            }
            else if(mnemonic == "START"){

                // ADD IC FOR MNEMONIC
                ic += "(" + opt.get_class("START") + "," + to_string(opt.get_code("START")) + ") ";
                
                // ADD IC FOR OPCODE1
                if(inst.hasOp1()){
                    lc = stoi(inst.op1);
                    ic += "(C," + inst.op1 + ") ";
                }
                else{
                    lc = 0;
                }
            
            }
            else if(mnemonic == "END"){
                ic += "(" + opt.get_class("END") + "," + to_string(opt.get_code("END")) + ") ";
            }
            else if(mnemonic == "ORIGIN"){
                
                // ADD IC FOR MNEMONIC
                string c = opt.get_class("ORIGIN");
                int mcCode = opt.get_code("ORIGIN");
                ic += "(" + c + "," + to_string(mcCode) + ") ";

                // UPDATE LC
                string op1 = inst.op1;
                string oprnd = "";
                string oprt = "";
                vector<string> v;

                for(auto i: op1){
                    if(i!='+'&& i!='-'){
                        oprnd += i;
                    }else{
                        oprt = i;
                        v.push_back(oprnd);
                        oprnd = "";
                    }
                }
                v.push_back(oprnd);

                int newLc = 0;
                for(auto i :symTab){
                    if(i.first==v[0]){
                        newLc = i.second;
                    }
                }
                if(oprt=="+"){
                    newLc += stoi(v[1]);
                }else{
                    newLc -= stoi(v[1]);
                }

                this->lc = newLc;

                // ADD IC FOR OP1
                int indx;
                for(int i=0; i<symTab.size(); i++){
                    if(symTab[i].first == v[0]){
                        indx = i;
                        break;
                    }
                }

                ic += "(S,"+to_string(indx)+")"+oprt+v[1];

            } 
            else if(mnemonic == "EQU"){

                // ADD IC FOR MNEMONIC
                ic += "(" + opt.get_class("EQU") + "," + to_string(opt.get_code("EQU")) + ") ";

                // CALCULATE NEW ADDRESS
                string op1 = inst.op1;
                string oprnd = "";
                string oprt = "";
                vector<string> v;

                for(auto i: op1){
                    if(i!='+'&& i!='-'){
                        oprnd += i;
                    }else{
                        oprt = i;
                        v.push_back(oprnd);
                        oprnd = "";
                    }
                }
                v.push_back(oprnd);
                
                int newAddr = 0;
                for(auto i :this->symTab){
                    if(i.first==v[0]){
                        newAddr = i.second;
                    }
                }
                if(oprt=="+"){
                    newAddr += stoi(v[1]);
                }else{
                    newAddr -= stoi(v[1]);
                }


                // FIND THE LABEL IN SYMBTAB
                for(int s=0; s<this->symTab.size(); s++){
                    if(symTab[s].first==inst.label){
                        symTab[s].second = newAddr;
                    }
                }

                // ADD IC FOR OP1
                int indx;
                for(int i=0; i<symTab.size(); i++){
                    if(symTab[i].first == v[0]){
                        indx = i;
                        break;
                    }
                }

                ic += "(S,"+to_string(indx)+")"+oprt+v[1];

            }
            else if(mnemonic == "DS"){

                // ADD IC FOR MNEMONIC
                ic += "(" + opt.get_class("DS") + "," + to_string(opt.get_code("DS")) + ") ";

                // FIND THE LABEL IN SYMBTAB
                for(int s=0; s<this->symTab.size(); s++){
                    if(symTab[s].first==inst.label){
                        symTab[s].second = lc;
                    }
                }

                // IC FOR OP1
                ic += "(C,"+inst.op1+") ";

            }
            else if(mnemonic == "DC"){

                // ADD IC FOR MNEMONIC
                ic += "(" + opt.get_class("DC") + "," + to_string(opt.get_code("DC")) + ") ";

                // FIND THE LABEL IN SYMBTAB
                for(int s=0; s<this->symTab.size(); s++){
                    if(symTab[s].first==inst.label){
                        symTab[s].second = lc;
                    }
                }

                // IC FOR OP1
                ic += "(C,"+inst.op1+") ";

            }
            else if(mnemonic == "LTORG"){

                for(int l=ptPtr; l<litTab.size(); l++){

                    litTab[l].second = this->lc;
                    this->lc++;

                    string lit = litTab[l].first;
                    lit = lit.substr(2, lit.length()-3);
                    ic += "(DL,01) (C," + lit + ") ";

                    iCode.push_back({lc,ic});

                    ic="";

                }

                ptPtr++;

            }

            if(ic!=""){
                if(opt.get_class(mnemonic)=="AD"){
                    iCode.push_back({-1,ic});
                }else{
                    iCode.push_back({lc,ic});
                }
                if(mnemonic=="DS"){
                    this->lc = lc+stoi(inst.op1);
                }else if(mnemonic=="DC"){
                    this->lc++;
                }
            }
            
        } 

    }
    

};

int main(){
    Tokenizer tknz;
    vector<Instruction> insts = tknz.TokenizeAll();
    for(auto i: insts){
        cout<<i.label<<"\t"<<i.mnemonic<<"\t"<<i.op1<<"\t"<<i.op2<<endl;
    }
    cout<<endl;
    Pass1 p1;
    p1.performPass1();
    p1.printICode();
    cout<<endl;
    p1.printLitTab();
    cout<<endl;
    p1.printSymTab();
    cout<<endl;
    return 0;
}


