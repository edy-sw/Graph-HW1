
#include "Graph.h"
#include <exception>
#include <iostream>
#include <algorithm>
#include <queue>
#include <limits>
#include <functional>

// Constructor
Graph::Graph(int nrVertices) {
    for (int i = 0; i < nrVertices; ++i) {
        inMap[i] = std::vector<int>();
        outMap[i] = std::vector<int>();
    }
}

Graph::Graph(const Graph& other) {
    // Deep copy of vertices and their connections
    inMap = other.inMap;
    outMap = other.outMap;
    costMap = other.costMap;
}

// Assignment operator
Graph& Graph::operator=(const Graph& other) {
    if (this != &other) { // Protect against self-assignment
        // Perform deep copy
        inMap.clear();
        outMap.clear();
        costMap.clear();

        inMap = other.inMap;
        outMap = other.outMap;
        costMap = other.costMap;
    }
    return *this;
}

// Adds an edge to the graph
void Graph::addEdge(int v1, int v2, int cost) {
    if (!isVertex(v1) or !isVertex(v2) or isEdge(v1, v2)) {
        throw std::exception();
    }
    costMap[std::make_pair(v1, v2)] = cost;
    outMap[v1].push_back(v2);
    inMap[v2].push_back(v1);
}

// Cgecks if a vertex exists
bool Graph::isVertex(int v) {
    if (inMap.find(v) != inMap.end()) {
        return true;
    }
    return false;
}

// Checks if an edge exists
bool Graph::isEdge(int v1, int v2) {
    return std::find(outMap[v1].begin(), outMap[v1].end(), v2) != outMap[v1].end();
}

// Returns a pair of iterators that iterate through the set of vertices
std::pair<std::map<int, std::vector<int>>::iterator, std::map<int, std::vector<int>>::iterator> Graph::setOfVertices() {
    return std::make_pair(outMap.begin(), outMap.end());
}

// Returns the number of vertices
int Graph::nrVertices() {
    return outMap.size();
}

// Returns the number of edges
int Graph::nrEdges() {
    return costMap.size();
}

// Returns the in degree of a vertex
int Graph::inDegree(int x) {
    return inMap[x].size();
}
// Returns the out degree of a vertex
int Graph::outDegree(int x) {
    return outMap[x].size();
}
// Returns the cost of an edge
int Graph::getCost(int x, int y) {
    if (!isEdge(x, y)) {
        throw std::exception();
    }
    return costMap[std::make_pair(x, y)];
}
// Modifies the cost of an edge
void Graph::changeCost(int x, int y, int val) {
    if (!isEdge(x, y)) {
        throw std::exception();
    }
    this->costMap[std::make_pair(x, y)] = val;
}
// Returns a pair of iterators that iterate through the set of inbound edges of a vertex
std::pair <std::vector<int>::iterator, std::vector<int>::iterator> Graph::inboundEdge(int x) {
    return std::make_pair(inMap[x].begin(), inMap[x].end());
}
// Returns a pair of iterators that iterate through the set of outbound edges of a vertex
std::pair<std::vector<int>::iterator, std::vector<int>::iterator> Graph::outboundEdge(int x) {
    return std::make_pair(outMap[x].begin(), outMap[x].end());
}
// Removes an edge
void Graph::removeEdge(int x, int y) {
    if (!isEdge(x, y)) {
        throw std::exception();
    }
    for (auto& elem : costMap) {
        if (y == elem.first.second and x == elem.first.first) {
            costMap.erase(elem.first);
            break;
        }
    }
    inMap[y].erase(std::find(inMap[y].begin(), inMap[y].end(), x));
    outMap[x].erase(std::find(outMap[x].begin(), outMap[x].end(), y));
}
// Adds a vertex
void Graph::addVertex(int val) {
    if (isVertex(val)) {
        throw std::exception();
    }
    inMap[val] = std::vector<int>();
    outMap[val] = std::vector<int>();
}
// Removes a vertex
void Graph::removeVertex(int val) {
    inMap.erase(inMap.find(val));
    outMap.erase(outMap.find(val));
    int len = costMap.size();
    std::vector<std::pair<int, int>> array;
    for (auto& el : costMap) {
        array.push_back(el.first);
    }
    int i = 0;
    std::map<std::pair<int, int>, int> newCostMap;
    while (i < len) {
        if (array[i].second == val or array[i].first == val) {
            costMap.erase(costMap.find(array[i]));
            array.erase(std::find(array.begin(), array.end(), array[i]));
            len--;
            continue;
        }
        int x = array[i].first;
        int y = array[i].second;
        if (x > val) x--;
        if (y > val) y--;
        newCostMap[std::make_pair(x, y)] = costMap[array[i]];
        i++;
    }
    costMap = newCostMap;
    std::map<int, std::vector<int>> newInMap;
    for (auto el : inMap) {
        auto c = std::find(el.second.begin(), el.second.end(), val);
        if (c != el.second.end()) {
            el.second.erase(c);
        }
    }
    for (i = 0; i < inMap.size(); ++i) {
        int x = i;
        if (x > val) x--;
        newInMap[x] = inMap[i];
        for (int j = 0; j < inMap[i].size(); ++j) {
            if (inMap[i][j] > val) {
                newInMap[x][j] = inMap[i][j] - 1;
            }
        }
    }
    std::map<int, std::vector<int>> newOutMap;
    for (auto el : outMap) {
        auto c = std::find(el.second.begin(), el.second.end(), val);
        if (c != el.second.end()) {
            el.second.erase(c);
        }
    }
    for (i = 0; i < outMap.size(); ++i) {
        int x = i;
        if (x > val) x--;
        newOutMap[x] = outMap[i];
        for (int j = 0; j < outMap[i].size(); ++j) {
            if (outMap[i][j] > val) {
                newOutMap[x][j] = outMap[i][j] - 1;
            }
        }
    }
    std::cout << '\n';
    inMap = newInMap;
    outMap = newOutMap;
}
// Returns a string representation of the graph
std::string Graph::toString() {
    std::string str;
    str += std::to_string(nrVertices()) + " " + std::to_string(nrEdges()) + "\n";
    for (auto i : costMap) {
        str += std::to_string(i.first.first) + " " + std::to_string(i.first.second) + " " + std::to_string(i.second) + "\n";
    }
    return str;
}

void Graph::DFS(int v, std::map<int, bool>& visited, std::vector<int>& component) {
    visited[v] = true;
    component.push_back(v);

    for (int neighbor : outMap[v]) {
        if (!visited[neighbor]) {
            DFS(neighbor, visited, component);
        }
    }
    for (int neighbor : inMap[v]) {
        if (!visited[neighbor]) {
            DFS(neighbor, visited, component);
        }
    }
}


std::vector<std::vector<int>> Graph::connectedComponents() {
    std::map<int, bool> visited;
    std::vector<std::vector<int>> components;

    for (auto const& pair : outMap) {
        visited[pair.first] = false;
    }

    for (auto const& pair : outMap) {
        int v = pair.first;
        if (!visited[v]) {
            std::vector<int> component;
            DFS(v, visited, component);
            components.push_back(component);
        }
    }

    return components;
}

std::pair<int, std::vector<int>> Graph::backwardsDijkstra(int startVertex, int endVertex) {
    if (!isVertex(startVertex) || !isVertex(endVertex)) {
        throw std::exception();
    }

    std::map<int, int> dist;
    std::map<int, int> next_node;
    for (auto const& pair : outMap) {
        dist[pair.first] = std::numeric_limits<int>::max();
    }

    // Min-heap priority queue: stores pairs of (distance, vertex)
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

    dist[endVertex] = 0;
    pq.push({0, endVertex});

    while (!pq.empty()) {
        int currentDist = pq.top().first;
        int currentVertex = pq.top().second;
        pq.pop();

        if (currentVertex == startVertex) {
            break; // We found the shortest path to the start vertex
        }

        if (currentDist > dist[currentVertex]) {
            continue;
        }

        // Search backwards using inbound edges
        for (int prevVertex : inMap[currentVertex]) {
            int edgeCost = getCost(prevVertex, currentVertex);
            if (dist[currentVertex] + edgeCost < dist[prevVertex]) {
                dist[prevVertex] = dist[currentVertex] + edgeCost;
                next_node[prevVertex] = currentVertex; // To reconstruct the path forwards
                pq.push({dist[prevVertex], prevVertex});
            }
        }
    }

    if (dist[startVertex] == std::numeric_limits<int>::max()) {
        // No path exists
        return {-1, {}};
    }

    // Reconstruct path
    std::vector<int> path;
    int current = startVertex;
    while (current != endVertex) {
        path.push_back(current);
        current = next_node[current];
    }
    path.push_back(endVertex);

    return {dist[startVertex], path};
}

std::pair<int, std::vector<std::pair<int, int>>> Graph::kruskalMST() {
    std::vector<std::pair<int, std::pair<int, int>>> edges;
    std::map<std::pair<int, int>, bool> added;

    for (auto const& pair : costMap) {
        int u = pair.first.first;
        int v = pair.first.second;
        int cost = pair.second;

        int minNode = std::min(u, v);
        int maxNode = std::max(u, v);

        if (!added[{minNode, maxNode}]) {
            edges.push_back({cost, {minNode, maxNode}});
            added[{minNode, maxNode}] = true;
        }
    }

    std::sort(edges.begin(), edges.end());

    std::map<int, int> parent;
    std::map<int, int> rank;
    for (auto const& pair : outMap) {
        parent[pair.first] = pair.first;
        rank[pair.first] = 0;
    }

    std::function<int(int)> findRoot = [&](int i) -> int {
        if (parent[i] == i)
            return i;
        return parent[i] = findRoot(parent[i]);
    };

    auto unify = [&](int i, int j) {
        int root_i = findRoot(i);
        int root_j = findRoot(j);

        if (root_i != root_j) {
            if (rank[root_i] < rank[root_j]) {
                parent[root_i] = root_j;
            } else if (rank[root_i] > rank[root_j]) {
                parent[root_j] = root_i;
            } else {
                parent[root_j] = root_i;
                rank[root_i]++;
            }
        }
    };

    int totalCost = 0;
    std::vector<std::pair<int, int>> mst;

    for (auto const& edge : edges) {
        int cost = edge.first;
        int u = edge.second.first;
        int v = edge.second.second;

        if (findRoot(u) != findRoot(v)) {
            totalCost += cost;
            mst.push_back({u, v});
            unify(u, v);
        }
    }

    return {totalCost, mst};
}

std::pair<int, std::vector<int>> Graph::tspApproximation() {
    auto mstResult = kruskalMST();
    std::vector<std::pair<int, int>> mstEdges = mstResult.second;

    std::map<int, std::vector<int>> mstAdj;
    for (auto edge : mstEdges) {
        mstAdj[edge.first].push_back(edge.second);
        mstAdj[edge.second].push_back(edge.first);
    }

    std::vector<int> path;
    std::map<int, bool> visited;
    
    if (outMap.empty()) {
        return {0, {}};
    }
    
    int startVertex = outMap.begin()->first;

    std::function<void(int)> mstDFS = [&](int u) {
        visited[u] = true;
        path.push_back(u);
        for (int v : mstAdj[u]) {
            if (!visited[v]) {
                mstDFS(v);
            }
        }
    };

    mstDFS(startVertex);
    path.push_back(startVertex); 

    int tspCost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int u = path[i];
        int v = path[i+1];
        
        if (isEdge(u, v)) {
            tspCost += getCost(u, v);
        } else if (isEdge(v, u)) {
            tspCost += getCost(v, u);
        } else {
            auto dijkstra_res = backwardsDijkstra(u, v);
            if (dijkstra_res.first != -1) {
                tspCost += dijkstra_res.first;
            } else {
                throw std::exception(); 
            }
        }
    }

    return {tspCost, path};
}