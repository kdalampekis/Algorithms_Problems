#include <iostream>  
#include <vector>  
#include <map>  
#include <queue>  
  
using namespace std;  
  
const int INF = 0xf3f3f3f; // Infinite distance value  
  
void dijkstra(const vector<vector<pair<int, int>>>& adj, vector<int>& distance_to, vector<int>& previous, int src) {  
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;   
  
    pq.push(make_pair(0, src));   
    distance_to[src] = 0;   
  
    while (!pq.empty()) {  
        int u = pq.top().second;   
        pq.pop();   
  
        for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {   
            int v = it->first;  
            int d = it->second;   
  
            if (distance_to[v] > distance_to[u] + d) {  
                distance_to[v] = distance_to[u] + d;   
                previous[v] = u;   
                pq.push(make_pair(distance_to[v], v));   
            }  
        }  
    }  
}  
  
  
int findShortestPath(const vector<vector<pair<int, int>>>& adj, const vector<int>& distance_to, const vector<int>& previous, int src, int dest, int t, const map<pair<int, int>, int>& edges, const vector<pair<int, pair<int, int>>>& not_available) {  
    int n = adj.size();  
    vector<bool> available(n, false);   
  
    int curr = src - 1;  
    int count = t;  
    int l = 0;  
    available[not_available[l].second.first] = true;   
    available[not_available[l].second.second] = true;   
    bool reached = false;   
  
    while (curr != dest - 1) {  
        while (count >= not_available[l].first && count < not_available[l + 1].first && !reached) {  
            if (available[curr] && available[previous[curr]]) {   
                auto iter = adj[curr].begin();  
                int temp, saved_node;  
                if (iter->first == previous[curr]) {   
                    ++iter;  
                    if (iter == adj[curr].end() && curr == dest - 1) {  
                        reached = true;  
                        continue;  
                    }  
                }  
                temp = distance_to[iter->first];  
                count = not_available[l + 1].first;  
                count += edges.at({curr, previous[curr]});  
                curr = previous[curr];  
                if (curr == dest - 1) reached = true;  
                continue;  
            } else {  
                count += edges.at({curr, previous[curr]});  
                curr = previous[curr];  
                if (curr == dest - 1) reached = true;  
            }  
        }  
        if (reached) {  
            break;  
        } else {  
            available[not_available[l].second.first] = false;  
            ++l;  
            available[not_available[l].second.second] = true;  
            continue;  
        }  
    }  
  
    if (curr != dest - 1) {  
        while (curr != dest - 1) {  
            count += edges.at({curr, previous[curr]});  
            curr = previous[curr];  
        }  
        return count - t;   
    } else {  
        return count - t;   
    }  
}  
  
int main() {  
    int n, m, t, p, src, dest;  
    cin >> n >> m;  
    cin >> src >> dest >> t >> p;   
    vector<vector<pair<int, int>>> adj(n);   
  
    vector<int> distance_to(n, INF);  
    vector<int> p_cities;  
  
    vector<int> previous(n, -1);   
  
    map<pair<int, int>, int> edges;   
  
    for (int i = 0; i < p; i++) {  
        int iter;  
        cin >> iter;   
        p_cities.push_back(iter - 1);   
    }  
  
    for (int i = 0; i < m; i++) {  
        int u, v, d;  
        cin >> u >> v >> d;  
        edges[{u - 1, v - 1}] = d;  
        edges[{v - 1, u - 1}] = d;  
  
        adj[u - 1].push_back({v - 1, d});  
        adj[v - 1].push_back({u - 1, d});  
    }  
  
    dijkstra(adj, distance_to, previous, dest - 1);  
  
    vector<pair<int, pair<int, int>>> not_available(p);  
    auto iter = p_cities.begin();  
    auto iter2 = iter + 1;  
    int l = 0;  
    int mc = 0;  
  
    while (iter2 != p_cities.end()) {  
        int counter = *iter;  
        int counter_ = *iter2;  
        not_available[l] = {mc, {counter, counter_}};  
        mc += edges.at({counter, counter_});  
  
        ++iter;  
        ++iter2;  
        ++l;  
    }  
    not_available[l] = {mc, {*iter, -1}};   
  
    int shortest_time = findShortestPath(adj, distance_to, previous, src, dest, t, edges, not_available);  
    cout << shortest_time << endl;   
  
    return 0;  
}  