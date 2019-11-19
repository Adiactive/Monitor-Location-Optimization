//
// Created by Rongzhi on 2019/11/13.
//

#ifndef ECE650_PRJ_VTXCOVER_H
#define ECE650_PRJ_VTXCOVER_H

#include <iostream>
#include <memory>
#include <vector>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
using namespace std;

struct ArgStruct {
    const int& vtxNum;
    const vector<int> edges;
    vector<int>* result;
};

void* CNF_SAT_VC(void* args) {
    auto* _args = (struct ArgStruct*) args;
    const int& _vtxNum = _args->vtxNum;
    const vector<int>& _edges = _args->edges;
    vector<int>* _result = _args->result;

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

        //set literals above diagonal to false
        for (int i = 0; i < _coverSize - 1 ; ++i) {
            for (int j = i + 1; j < _coverSize ; ++j) {
                solver->addClause(~allLit[i * _coverSize + j]);
            }
        }

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


    //write the result to array
    int count = _coverSize;
    for (int i = 0; i < _vtxNum; ++i) {
        for (int j = 0; j < _coverSize; ++j) {
            if (!Minisat::toInt(solver->modelValue(allLit[i * _coverSize + j]))) {
                _result->push_back(i);
                count--;
                if (!count)
                    return nullptr;
            }
        }
    }
    return nullptr;
}

void* APPROX_VC_1 (void* args) {
    auto* _args = (struct ArgStruct*) args;
    const int& _vtxNum = _args->vtxNum;
    vector<int> _edges = _args->edges;
    vector<int>* _result = _args->result;

    int* degree = new int[_vtxNum];
    while (!_edges.empty()) {
        //get the vertex of highest degree
        int maxDegree = 0;
        int maxDegVtx = 0;
        memset(degree, 0, sizeof(int) * _vtxNum);
        for (auto e : _edges) {
            degree[e] += 1;
            if (degree[e] > maxDegree || (degree[e] == maxDegree && e < maxDegVtx)) {
                maxDegree = degree[e];
                maxDegVtx = e;
            }
        }

        _result->push_back(maxDegVtx);

        //delete all edges incident on that vertex
        for (auto it = _edges.begin(); it != _edges.end();) {
            if (*it == maxDegVtx || *(it + 1) == maxDegVtx)
                _edges.erase(it, it + 2);
            else
                it += 2;
        }
    }

    sort(_result->begin(), _result->end());
    delete[] degree;
    return nullptr;
}

void* APPROX_VC_2 (void* args) {
    auto* _args = (struct ArgStruct*) args;
    vector<int> _edges = _args->edges;
    vector<int>* _result = _args->result;

    while (!_edges.empty()) {
        //pick an edge, add both their vertex to vertex cover
        //and delete all edges attached to them
        int u = _edges[0];
        int v = _edges[1];

        for (auto it = _edges.begin(); it != _edges.end();) {
            if (*it == u || *(it + 1) == u || *it == v || *(it + 1) == v)
                _edges.erase(it, it + 2);
            else
                it += 2;
        }
        _result->push_back(u);
        _result->push_back(v);
    }

    sort(_result->begin(), _result->end());
    return nullptr;
}

#endif //ECE650_PRJ_VTXCOVER_H
