#include <bits/stdc++.h>
using namespace std;

int main() {
    int K, N;
    cout<<"K = ";
    cin>>K;
    cout<<"N = ";
    cin>>N;
    priority_queue<int, vector<int>, greater<int> > minHeap;
    cout<<"Scores : "<<endl;
    for(int i=0;i<N;i++){
        int x;
        cin>>x;

        if(minHeap.size()<K){
            minHeap.push(x);

            if(minHeap.size()<K)
                cout<<-1<<endl;
            else
                cout<<minHeap.top()<<endl;
        }
        else{
            if(x>minHeap.top()){
                minHeap.pop();
                minHeap.push(x);
            }
            cout<<minHeap.top()<<endl;
        }
    }
    return 0;
}

