class Solution {
  public:
    int spanningTree(int V, vector<vector<int>>& edges) {
        
        vector<pair<int,int>> adj[V];
        
        for(auto &edge : edges){
            int u = edge[0];
            int v = edge[1];
            int wt = edge[2];
            
            adj[u].push_back({v, wt});
            adj[v].push_back({u, wt});
        }
        
        priority_queue<
            pair<int,int>, 
            vector<pair<int,int>>, 
            greater<pair<int,int>>
        > pq;
        
        vector<int> visited(V, 0);
        
        pq.push({0, 0});
        
        int mstWeight = 0;
        
        while(!pq.empty()) {
            auto top = pq.top();
            pq.pop();
            
            int wt = top.first;
            int node = top.second;
            
            if(visited[node]) continue;
            
            visited[node] = 1;
            mstWeight += wt;
            
            for(auto &neighbour : adj[node]) {
                int adjNode = neighbour.first;
                int edgeWeight = neighbour.second;
                
                if(!visited[adjNode]) {
                    pq.push({edgeWeight, adjNode});
                }
            }
        }
        
        return mstWeight;
    }
};
