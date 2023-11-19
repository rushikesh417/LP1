#include<bits/stdc++.h>
using namespace std;

void FIFO(int pg[] ,int pn ,int  fn){

    vector<int>vec(fn,-1);
    queue<int>q;

    int faults=0;
    int hit=0;

    cout<<endl;
    cout<<"------------------------------------"<<endl;
    cout<<"Frame structure of FIFO is as Follows ..."<<endl;
    for(int i=0;i<pn;i++){

        int inComing= pg[i];
        int cnt=0;

        for(int j=0;j<fn;j++){
            if(vec[j]==-1){
                q.push(inComing);
                vec[j]=inComing;
                faults++;
                break;
            }
            else if(vec[j]==inComing){
                hit++;
            }
            else if(inComing != vec[j]){
                cnt++;
            }
        }

        if(cnt==fn){
            int qfront = q.front();q.pop();

            for(int j=0;j<fn;j++){
                if(vec[j] == qfront){
                    vec[j] = inComing;
                    q.push(inComing);
                    faults++;
                    break;
                }
            }
        }

        for(auto it:vec){
            cout<<"("<<it<<")"<<" ";
        }
        cout<<endl;
        
    }
    cout<<endl;
    cout << "No. of hits = " << hit << endl;
    cout << "No. of misses = " << pn - hit << endl;
    
}


bool search(int key, vector<int>& fr)
{
    for (int i = 0; i < fr.size(); i++)
        if (fr[i] == key)
            return true;
    return false;
}

int predict(int pg[], vector<int>& fr, int pn, int index)
{
    
    int res = -1, farthest = index;
    for (int i = 0; i < fr.size(); i++) {
        int j;
        for (j = index; j < pn; j++) {
            if (fr[i] == pg[j]) {
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }
 
       
        if (j == pn)
            return i;
    }

    if(res==-1){
        return 0;
    }

    return res;
}

void optimal(int pg[] ,int pn ,int  fn){

    vector<int> fr;
    
    int hit = 0;
    cout<<endl;
    cout<<"------------------------------------"<<endl;
    cout<<"Frame structure of Optimal is as Follows ..."<<endl;
    for (int i = 0; i < pn; i++) {
 
        if (search(pg[i], fr)) {
            hit++;
        }
        else if (fr.size() < fn){
            fr.push_back(pg[i]);
        }
        else {
            int j = predict(pg, fr, pn, i + 1);
            fr[j] = pg[i];
        }

        for(auto it:fr){
            cout<<"("<<it<<")"<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    cout << "No. of hits = " << hit << endl;
    cout << "No. of misses = " << pn - hit << endl;


}

int lruprediction(int pg[] ,vector<int>& fr ,int pn , int index){

    int ans=0;
    int mini=index+1;
    for(int i=0;i<fr.size();i++){
        
        for(int j=index ;j>=0;j--){
            if(fr[i] == pg[j]){
                if(j < mini){
                    mini=j;
                    ans=i;
                }
                break;
            }
        }
        
    }

    return ans;

}

void lru(int pg[], int pn, int fn){

    vector<int> fr;
    
    int hit = 0;
    cout<<endl;
    cout<<"------------------------------------"<<endl;
    cout<<"Frame structure of lruu is as Follows ..."<<endl;
    for (int i = 0; i < pn; i++) {
 
        if (search(pg[i], fr)) {
            hit++;
        }
        else if (fr.size() < fn){
            fr.push_back(pg[i]);
        }
        else {
            int j = lruprediction(pg, fr, pn, i - 1);
            fr[j] = pg[i];
        }

        for(auto it:fr){
            cout<<"("<<it<<")"<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    cout << "No. of hits = " << hit << endl;
    cout << "No. of misses = " << pn - hit << endl;


}


int main(){

    int pg[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int pn = 20; 
    int fn = 4 ; 
    FIFO(pg,pn, fn);
    optimal(pg,pn, fn);
    lru(pg,pn, fn);

    return 0;
}