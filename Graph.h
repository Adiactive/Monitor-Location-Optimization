//
// Created by Rongzhi on 2019/10/4.
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
    bool exist(int u, int v)const;
    bool insert(int u, int v);
    bool path(int src, int dst);

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

bool Graph::insert(int u, int v) {
    head[u].push_back(v);
    return true;
}

bool Graph::exist(int u, int v) const {
    if (u < 0 || v < 0 || u > size - 1 || v > size - 1 || u == v)
        return false;
    auto it = find(head[u].cbegin(), head[u].cend(), v);
    return (it != head[u].cend());
}

bool Graph::path(int src, int dst) {
    int previous[size];
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
    int visited[size];
    for (int i = 0; i < size; i++)
        visited[i]=false;
    queue<int> q;
    visited[src] = true;
    q.push(src);
    while (!q.empty()){
        src = q.front();
        if (src == dst){
            return true;
        }
        q.pop();
        list<int>::const_iterator iter;
        for (iter = head[src].cbegin(); iter != head[src].cend(); ++iter)
            if (!visited[*iter]) {
                previous[*iter] = src;
                visited[*iter] = true;
                q.push(*iter);
            }
    }
    return false;
}

#endif //ECE650_A2_GRAPH_H
