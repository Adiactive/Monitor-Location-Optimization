//
// Created by Rongzhi on 2019/11/13.
//

#ifndef ECE650_PRJ_VTXCOVER_H
#define ECE650_PRJ_VTXCOVER_H

#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include<algorithm>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
using namespace std;

//TIME_OUT is defined in second
#define TIME_OUT 300
#define IS_TIME_OUT (-1)

struct ArgStruct {
    const int& vtxNum;
    const vector<int> edges;
    vector<int>* result;
};

void checkTimeout(std::unique_ptr<Minisat::Solver>& _solver, bool& _isTimeout, bool& _isSolved) {
    for (int i = 0; i < TIME_OUT * 10; ++i) {
        if (_isSolved)
            return;
        usleep(100000);
    }
    _isTimeout = true;
    _solver->interrupt();
}

void* CNF_SAT_VC(void* args) {
    //allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    bool res;
    int _coverSize = 0; //vertex cover size
    vector<Minisat::Lit> allLit;
    Minisat::vec<Minisat::Lit> vecLit;

    //create a thread to check for timeout
    bool isTimeout = false;
    bool isSolved = false;
    thread t(checkTimeout, ref(solver), ref(isTimeout), ref(isSolved));

    auto* _args = (struct ArgStruct*) args;
    const int& _vtxNum = _args->vtxNum;
    const vector<int>& _edges = _args->edges;
    vector<int>* _result = _args->result;

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

        //Fix all the solutions location
        for (int l = 1; l < _coverSize; ++l) {
            for (int i = 0; i < l; ++i) {
                for (int j = 0; j < _vtxNum; ++j) {
                    for (int k = j + 1; k < _vtxNum; ++k) {
                        solver->addClause(~allLit[j * _coverSize + l], ~allLit[k * _coverSize + i]);
                    }
                }
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

        solver->clearInterrupt();
        res = solver->solve();
        if (res) {
            //terminate checkTimeout thread
            isSolved = true;
            t.join();
            break;
        }
        else if (isTimeout) {
            _result->push_back(IS_TIME_OUT);
            t.join();
            return nullptr;
        }
        else {
            _coverSize++;
            allLit.clear();
            solver.reset(new Minisat::Solver());
        }
    }

    //write the solution to results[0]
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

    //should never reach here
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

        //write the solution to results[1]
        _result->push_back(maxDegVtx);

        //delete all edges incident on that vertex
        for (auto it = _edges.begin(); it != _edges.end();) {
            if (*it == maxDegVtx || *(it + 1) == maxDegVtx)
                _edges.erase(it, it + 2);
            else
                it += 2;
        }
    }

    //sort solution by increasing order
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
        //write the solution to results[2]
        _result->push_back(u);
        _result->push_back(v);
    }

    //sort solution by increasing order
    sort(_result->begin(), _result->end());
    return nullptr;
}

#endif //ECE650_PRJ_VTXCOVER_H
