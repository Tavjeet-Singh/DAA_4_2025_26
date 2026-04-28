#include <bits/stdc++.h>
using namespace std;

struct Job {
    int id, d, p;
};
bool cmp(Job a, Job b) {
    return a.p > b.p;
}
int main() {
    int n;
    cin >> n;
    vector<int> deadline(n), profit(n);
    for(int i = 0; i < n; i++) {
    	cin >> deadline[i];
	}
    for(int i = 0; i < n; i++) {
    	cin >> profit[i];
	}
    vector<Job> jobs(n);
    for(int i = 0; i < n; i++) {
        jobs[i] = {i + 1, deadline[i], profit[i]};
    }
    sort(jobs.begin(), jobs.end(), cmp);
    int maxD = 0;
    for(int i = 0; i < n; i++) {
    	maxD = max(maxD, jobs[i].d);
	}
    vector<int> slot(maxD + 1, -1);
    int cnt = 0, total = 0;
    for(int i = 0; i < n; i++) {
        for(int j = jobs[i].d; j > 0; j--) {
            if(slot[j] == -1) {
                slot[j] = i;
                cnt++;
                total += jobs[i].p;
                break;
            }
        }
    }
    cout << "[" << cnt << " , " << total << "]";
}
