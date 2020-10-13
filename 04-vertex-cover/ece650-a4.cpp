#include <iostream>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <vector>

#include "Graph.h"
using namespace std;

//for error handling
struct Exception : std::runtime_error {
    explicit Exception(const char *msg) : runtime_error(msg) {}
};

// parse command option
char parseCmd(istringstream &input) {
    char cmd;
    input >> cmd;
    if (input.fail() || (cmd != 'V' && cmd != 'E' && cmd != 's')) {
        throw Exception("unknown command option, expect `V, E, s`");
    }
    return cmd;
}

int main() {
    Graph* graph = nullptr;
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
                delete graph;
                graph = new Graph(vtxNum);
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
                        else if (graph->exist(src, dst))
                            throw Exception("add duplicate edge(s)");
                        else {
                            edges.push_back(src);
                            edges.push_back(dst);
                        }
                    }
                    input >> c;
                }

                //insert edges into the graph
                for (size_t i = 0; i < edges.size() ; i += 2) {
                    graph->insert(edges[i], edges[i + 1]);
                    graph->insert(edges[i + 1], edges[i]);
                }

                //compute minimum-sized vertex cover
                graph->getVtxCover(edges);

                edges.clear();
            }

            /*
             * this is a existing functionality of a2 to calculate shortest path
             * and is reserved in a4 to maintain functional completeness
             * may not be used
             */
            //search path
            else {
                int src, dst;
                input >> src >> dst;
                if (src < 0 || dst < 0 || src > vtxNum- 1 || dst > vtxNum - 1)
                    throw Exception("vertex index out of range");
                if (!graph->path(src, dst))
                    throw Exception("no path Exist");
            }
        }
        catch(Exception &exp){
            cerr << "Error: " << exp.what() << endl;
        }
    }
    return 0;
}

