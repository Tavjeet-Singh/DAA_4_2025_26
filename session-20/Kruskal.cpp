class Solution {
  public:
    
    int findParent(int node, vector<int>& parent) {
        if (parent[node] == node)
            return node;
        return parent[node] = findParent(parent[node], parent);
    }
    
    void unionSet(int u, int v, vector<int>& parent, vector<int>& rank) {
        u = findParent(u, parent);
        v = findParent(v, parent);
        
        if (u == v) return;
        
        if (rank[u] < rank[v]) {
            parent[u] = v;
        } 
        else if (rank[u] > rank[v]) {
            parent[v] = u;
        } 
        else {
            parent[v] = u;
            rank[u]++;
        }
    }
    
    int kruskalsMST(int V, vector<vector<int>> &edges) {
        
        sort(edges.begin(), edges.end(), 
             [](vector<int>& a, vector<int>& b) {
                 return a[2] < b[2];
             });
        
        vector<int> parent(V);
        vector<int> rank(V, 0);
        
        for (int i = 0; i < V; i++)
            parent[i] = i;
        
        int mstWeight = 0;
        int edgesUsed = 0;
        
        for (auto &edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int wt = edge[2];
            
            if (findParent(u, parent) != findParent(v, parent)) {
                mstWeight += wt;
                unionSet(u, v, parent, rank);
                edgesUsed++;
                
                if (edgesUsed == V - 1) 
                    break;
            }
        }
        
        return mstWeight;
    }
};
