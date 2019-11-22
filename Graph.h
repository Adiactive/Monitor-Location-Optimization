//
// Created by Rongzhi Gu on 2019/10/20.
//

/*
 * some member variables and functions are from a2
 * and are reserved to maintain functional completeness
 * may not be used in a4
 */

#ifndef ECE650_A4_GRAPH_H
#define ECE650_A4_GRAPH_H

#include <list>
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
using namespace std;

class Graph {
public:
    explicit Graph(int mSize);
    ~Graph();
    bool exist(int u, int v)const;
    void insert(int u, int v);
    bool path(int src, int dst);
    void getVtxCover(const vector<int>& _edges);

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

void Graph::insert(int u, int v) {
    if (!this->exist(u, v) && u != v)
        head[u].push_back(v);
}

bool Graph::exist(int u, int v) const {
    if (u < 0 || v < 0 || u > size - 1 || v > size - 1 || u == v)
        return false;
    auto it = find(head[u].cbegin(), head[u].cend(), v);
    return (it != head[u].cend());
}

bool Graph::path(int src, int dst) {
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

void Graph::getVtxCover(const vector<int>& _edges) {
    int _vtxNum = this->size;
    //allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    bool res;
    int _coverSize = 0; //vertex cover size
    vector<Minisat::Lit> allLit;
    Minisat::vec<Minisat::Lit> vecLit;
    while (true) {
        //add literals
        for (int i = 0; i < _vtxNum * _coverSize; ++i)
            allLit.push_back(Minisat::mkLit(solver -> newVar()));

        //at least one vertex is the ith vertex in the vertex cover
        for (int i = 0; i < _coverSize; ++i) {
            for (int j = 0; j < _vtxNum; ++j) {
                vecLit.push(allLit[i + j * _coverSize]);
            }
            solver->addClause(vecLit);
            vecLit.clear();
        }

        //no one vertex can appear twice in a vertex cover
        for (int i = 0; i < _vtxNum; ++i) {
            for (int j = 0; j < _coverSize; ++j) {
                for (int k = j + 1; k < _coverSize; ++k) {
                    solver->addClause(~allLit[i * _coverSize + j], ~allLit[i * _coverSize + k]);
                }
            }
        }

        //no more than one vertex appears in the mth position of the vertex cover
        for (int i = 0; i < _coverSize; ++i) {
            for (int j = 0; j < _vtxNum; ++j) {
                for (int k = j + 1; k < _vtxNum; ++k) {
                    solver->addClause(~allLit[i + j * _coverSize], ~allLit[i + k * _coverSize]);
                }
            }
        }

        //Every edge is incident to at least one vertex in the vertex cover
        for (size_t i = 0; i < _edges.size(); i += 2) {
            int src = _edges[i], dst = _edges[i + 1];
            for (int j = 0; j < _coverSize; ++j) {
                vecLit.push(allLit[src * _coverSize + j]);
                vecLit.push(allLit[dst * _coverSize + j]);
            }
            solver->addClause(vecLit);
            vecLit.clear();
        }

        res = solver->solve();
        if (res)
            break;
        else {
            _coverSize++;
            allLit.clear();
            solver.reset(new Minisat::Solver());
        }
    }

    //print vertex cover
    int count = _coverSize;
    for (int i = 0; i < _vtxNum; ++i) {
        for (int j = 0; j < _coverSize; ++j) {
            if (!Minisat::toInt(solver->modelValue(allLit[i * _coverSize + j]))) {
                cout << i;
                count--;
                if (count)
                    cout << ' ';
                break;
            }

        }
    }
    cout << endl;
}


#endif //ECE650_A4_GRAPH_H
