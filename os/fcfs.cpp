#include<bits/stdc++.h>
using namespace std;

class Process{
    public:
    char pN;
    int arrivalT;
    int burstT;
    int completionT;
    int turnAroundT;
    int waitingT;

    Process(char pN, int at, int bt){
        this->pN = pN;
        this->arrivalT = at;
        this->burstT = bt;
        this->completionT = -1;
        this->turnAroundT = -1;
        this->waitingT = -1;
    }

};

class FCFSScheduling{

    public:
    vector<Process> Processes;

    void calCompletionTime(){
        Processes[0].completionT = Processes[0].burstT;
        for(int i=1; i<Processes.size(); i++){
            if(Processes[i-1].completionT >= Processes[i].arrivalT)
                Processes[i].completionT = Processes[i-1].completionT + Processes[i].burstT;
            else{
                Processes[i].completionT = (Processes[i].arrivalT - Processes[i-1].completionT)+Processes[i-1].completionT + Processes[i].burstT;
            }
        }
    }

    void calTurnAroundTime(){
        for(int i=0; i<Processes.size(); i++){
            Processes[i].turnAroundT = Processes[i].completionT - Processes[i].arrivalT;
        }
    }

    void calWaitingTime(){
        for(int i=0; i<Processes.size(); i++){
            Processes[i].waitingT = Processes[i].turnAroundT - Processes[i].burstT;
        }
    }

    void process(){
        calCompletionTime();
        calTurnAroundTime();
        calWaitingTime();
    }

    void inputProcesses(int n){
        for(int i=0; i<n; i++){
            char pn;
            int at, bt;
            // cout<<"ProcessNo : ";
            // cin>>pn;
            cout<<"Arival Time : ";
            cin>>at;
            cout<<"Burst Time : ";
            cin>>bt;
            cout<<endl;
            Process p(pn, at, bt);
            Processes.push_back(p);
        }
    }

    void displayProcesses(){
        for(auto i: Processes){
            cout<<i.pN<<"\t"<<i.arrivalT<<"\t"<<i.burstT<<"\t"<<i.completionT<<"\t"<<i.turnAroundT<<"\t"<<i.waitingT<<endl;
        }
    }

    void displayGantt(){
        cout<<"0-"<<Processes[0].completionT<<":"<<Processes[0].pN<<endl;
        for(int i=1; i<Processes.size(); i++){
            cout<<Processes[i-1].completionT<<"-"<<Processes[i].completionT<<":"<<Processes[i].pN<<endl;
        }
    }

};

int main(){
    FCFSScheduling fcfs;
    fcfs.inputProcesses(4);
    fcfs.process();
    fcfs.displayProcesses();
    fcfs.displayGantt();
    return 0;
}


// Arival Time : 0
// Burst Time : 2

// Arival Time : 1
// Burst Time : 2

// Arival Time : 5
// Burst Time : 3

// Arival Time : 6
// Burst Time : 4