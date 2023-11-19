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
    int remainingT;
    int priority;

    Process(char pN, int at, int bt){
        this->pN = pN;
        this->arrivalT = at;
        this->burstT = bt;
        this->priority = -1;
        this->completionT = -1;
        this->turnAroundT = -1;
        this->waitingT = -1;
        this->remainingT = bt;
    }

};

class RRScheduling{
    public:
    vector<Process> Processes;
    int quantumT;

    RRScheduling(int qt){
        this->quantumT = qt;
    }

    void calCompletionTime(){

        sort(Processes.begin(), Processes.end(), [](const Process& a, const Process& b) {
            return a.arrivalT < b.arrivalT;
        });

        int currentT = 0;
        queue<Process> q;
        vector<Process> temp;
        
        while(!q.empty() || !Processes.empty()){

            while(!Processes.empty() && Processes.front().arrivalT<=currentT){
                q.push(Processes.front());
                Processes.erase(Processes.begin());
            }

            if(!q.empty()){

                Process p = q.front();
                q.pop();

                int executionT = min(quantumT, p.remainingT);
                currentT += executionT;
                p.remainingT -= executionT;

                if(p.remainingT>0){
                    while(!Processes.empty() && Processes.front().arrivalT<=currentT){
                        q.push(Processes.front());
                        Processes.erase(Processes.begin());
                    }
                    q.push(p);
                }
                else{
                    p.completionT = currentT;
                    temp.push_back(p);
                }

            }else{
                currentT+=quantumT;
            }
        }

        Processes = temp;

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
            Process pr(pn, at, bt);
            Processes.push_back(pr);
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
    RRScheduling rr(2);
    rr.inputProcesses(4);
    rr.process();
    rr.displayProcesses();
    rr.displayGantt();
    return 0;
}