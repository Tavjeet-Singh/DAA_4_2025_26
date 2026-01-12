
/* 24BCS10329 */
/* T(n) = 3T(n/2) + n log n */
/* Time Complexity: O(n2) */

#include <bits/stdc++.h>
using namespace std::chrono;

long long operations = 0;
int maxDepth = 0;

void complexRec(int n, int depth) {

    maxDepth = max(maxDepth, depth);
    operations++;

    if (n <= 2) {
        return;
    }

    int p = n;
    while (p > 0) {
        operations++;
        vector<int> temp(n);
        for (int i = 0; i < n; i++) {
            temp[i] = i ^ p;
            operations++;
        }
        p >>= 1;
    }

    vector<int> small(n);
    for (int i = 0; i < n; i++) {
        small[i] = i * i;
        operations++;
    }

    reverse(small.begin(), small.end());
    operations += n;

    complexRec(n / 2, depth + 1);
    complexRec(n / 2, depth + 1);
    complexRec(n / 2, depth + 1);
}

int main() {

    int n = 62;
    operations = 0;
    maxDepth = 0;

    auto start = high_resolution_clock::now();
    complexRec(n, 1);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    cout << "Operation Count = " << operations << endl;
    cout << "Max Depth = " << maxDepth << endl;
    cout << "Time Taken = " << duration.count() << " ms" << endl;

    return 0;
}

