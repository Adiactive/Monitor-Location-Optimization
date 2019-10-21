//
// Created by Rongzhi on 2019/10/20.
//

#ifndef ECE650_A2_GRAPH_H
#define ECE650_A2_GRAPH_H

#include <list>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class Graph {
public:
    explicit Graph(int mSize);
    ~Graph();
    bool Exist(int u, int v)const;
    void Insert(int u, int v);
    bool Path(int src, int dst);

protected:
    vector<list<int>> head;
    int size;

    bool BFS(int src, int dst, int *previous);
};

Graph::Graph(int mSize) {
    size = mSize;
    head = vector<list<int>>(size);
}

Graph::~Graph() = default;

void Graph::Insert(int u, int v) {
    if (!this->Exist(u, v) && u != v)
        head[u].push_back(v);
}

bool Graph::Exist(int u, int v) const {
    if (u < 0 || v < 0 || u > size - 1 || v > size - 1 || u == v)
        return false;
    auto it = find(head[u].cbegin(), head[u].cend(), v);
    return (it != head[u].cend());
}

bool Graph::Path(int src, int dst) {
    int previous[size];// store the previous one of a vertex in a path
    for (int i = 0; i < size; ++i)
        previous[i] = -1;
    if (BFS(dst, src, previous)){
        int cur = src;
        do {
            cout << cur << '-';
            cur = previous[cur];
        }while (cur != dst);
        cout << dst << endl;
        return true;
    }
    else
        return false;
}

bool Graph::BFS(int src, int dst, int *previous) {
    // for identical src and dst cases
    if (src == dst) {
        previous[dst] = src;
        return true;
    }

    int visited[size];
    for (int i = 0; i < size; i++)
        visited[i]=false;
    queue<int> q;
    visited[src] = true;
    q.push(src);
    while (!q.empty()){
        src = q.front();
        if (src == dst)
            return true;
        q.pop();
        for (auto val : head[src])
            if (!visited[val]) {
                previous[val] = src;
                visited[val] = true;
                q.push(val);
            }
    }
    return false;
}

#endif //ECE650_A2_GRAPH_H
