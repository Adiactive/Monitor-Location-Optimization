#include <iostream>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <vector>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
using namespace std;

//for error handling
struct Exception : std::runtime_error {
    explicit Exception(const char *msg) : runtime_error(msg) {}
};

void getVtxCover(int _vtxNum, const vector<int>& _edges) {
    //allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    bool res;
    int _coverSize = 1; //vertex cover size
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

// parse command option
char parseCmd(istringstream &input) {
    char cmd;
    input >> cmd;
    if (input.fail() || (cmd != 'V' && cmd != 'E')) {
        throw Exception("unknown command option, expect `V, E`");
    }
    return cmd;
}

int main() {
    string line;
    int vtxNum = 0;
    while (!cin.eof()) {
        try {
            // read a line of input until EOL and store in a string
            // create an input stream based on the line to parse it
            getline(cin, line);
            if (line.empty()) {
                if (!cin.eof())
                    throw Exception("no command detected");
                else
                    continue;
            }
            istringstream input(line);
            char cmd = parseCmd(input);

            //read vertex number
            if (cmd == 'V'){
                input >> vtxNum;
                if (input.fail() || vtxNum < 0)
                    throw Exception("wrong vertex number");
            }

            //read edges
            else if (cmd == 'E') {
                int src = 0, dst = 0;
                vector<int> edges; // store src and dst vertex index in case error input are detected
                char c = 0; // read separators
                input >> c;
                while (input) {
                    if ((c >= '0') && (c <= '9')) {
                        input.putback(c);
                        input >> src;
                        input >> c; //skip comma
                        input >> dst;
                        if (src < 0 || dst < 0 || src > vtxNum - 1 || dst > vtxNum - 1) {
                            edges.clear();
                            throw Exception("vertex index out of range");
                        }
                        else {
                            edges.push_back(src);
                            edges.push_back(dst);
                        }
                    }
                    input >> c;
                }

                getVtxCover(vtxNum, edges);

                edges.clear();
            }
        }
        catch(Exception &exp){
            cerr << "Error: " << exp.what() << endl;
        }
    }
    return 0;
}

