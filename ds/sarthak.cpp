/*
----> Sarthak Nirgude <----
        ◉_◉
----|-----------------|----
*/
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

class Election{
    int total;
    vector<bool>state;
    public:
    Election(int total_process){
        this->total = total_process;
        state = vector<bool>(total,true);
    }
    void up(int pos){
        state[pos-1] = true;
    }
    void down(int pos){
        state[pos-1] = false;
    }
    void Ring(int pos){
        if(state[pos-1] == false){
            cout << "p"<<pos<<"cannot start election as it is down"<<endl;
            return;
        }

        cout << " >>>Election started by p"<<pos<<endl;
        int start = pos-1;
        int ptr = (start+1)%total;
        int last_true = start;
        cout << "---------------------------------------------" << endl;
        while(ptr != start){
            if(state[ptr] == true){
                cout<<"p"<<last_true+1<<" sends election message to p"<<ptr+1<<endl;
                last_true = ptr;
            }
            Sleep(1000);
            ptr = (ptr + 1) % total;
        }
        cout<<"p"<<last_true + 1 << " sends election message to p"<<start+1<<endl;
       
        int max_process = -1;
        for(int i = total - 1;i >= 0; --i){
            if(state[i] == true){
                max_process = i;
                break;
            }
        }
        char smiley = 2;
        cout << "---------------------------------------------" << endl;
        cout << "Election is won by : p"<<max_process+1<<" "<<smiley<<endl;
        cout << "---------------------------------------------" << endl;
        cout << "p"<<max_process+1<<" informs everyone that it is new coordinator "<<endl;
        cout << "==============================================" << endl;
        
    }
    void Bully(int pos){
        if(pos > total){
            return;
        }
        if(state[pos-1] == false){
            cout << "p"<<pos<<" cannot start election as it is down"<<endl;
            return;
        }
        cout << ">>>Election started by p"<<pos<<endl;
        cout << "---------------------------------------------" << endl;
        for(int i=pos;i < total;i++){
            cout<<"Election message sent p"<<pos << " to p"<<i+1<<endl;
            Sleep(1000);
        }
        for(int i=pos;i < total;i++){
            if(state[i] == true){
                cout<<"p"<<i+1<<" responds OK to p"<<pos<<endl;
                Sleep(1000);
            }
        }
        cout << "---------------------------------------------" << endl;
        for(int i=pos;i < total;i++){
            if(state[i] == true){
                Bully(i+1);
                return;
            }
        }
        int max_process = -1;
        for(int i=total-1;i >= 0;i--){
            if(state[i] == true){
                max_process = i;
                break;
            }
        }
        char smiley = 1;
        cout << "---------------------------------------------" << endl;
        cout << "Election is won by : p"<<max_process+1<<" "<<smiley<<endl;
        cout << "---------------------------------------------" << endl;
        cout << "p"<<max_process+1<<" informs everyone that it is new coordinator "<<endl;
        cout << "==============================================" << endl;
    }
};
int main(){
    int n;
    while(1){
        int choice;
        cout<<"1.Ring Algorithm\n2.Bully Algorithm\n3.Exit"<<endl;
        cout << "Enter your choice : ";
        cin >> choice;
        if(choice == 1){
            cout << "Enter total no. of process :";
            cin >> n;
            Election r(n);
            r.down(n);
            r.Ring(2);
        }
        else if(choice == 2){
            cout << "Enter total no. of process :";
            cin >> n;
            Election b(n);
            b.down(n);
            b.Bully(2);
        }
        else{
            exit(0);
        }
    }
    return 0;
}