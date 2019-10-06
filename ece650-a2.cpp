// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <vector>
#include <sstream>
#include "Graph.h"
using namespace std;

//for error handling
struct Exception : runtime_error {
    explicit Exception(const char *msg) : runtime_error(msg) {}
};

// parse command option
char command_parser(istringstream &input) {
    char opt;
    input >> opt;
    if (input.fail() || (opt != 'V' && opt != 'E' && opt != 's')) {
        throw Exception("unknown command option, expect `V, E, s`");
    }
    return opt;
}

int main(int argc, char** argv) {
    Graph* graph = nullptr;
    int vtxNum = 0;
    // read from stdin until EOF
    while (!cin.eof()) {
        std::string line;
        getline(cin, line);
        if (line.empty()) {
            continue;
        }
        try {
            // read a line of input until EOL and store in a string
            // create an input stream based on the line to parse it
            std::istringstream input(line);
            char cmd = command_parser(input);

            //initialize graph
            if (cmd == 'V'){
                input >> vtxNum;
                if (input.fail())
                    throw Exception("can not parse vertex number");
                delete graph;
                graph = new Graph(vtxNum);
            }

            //add edge
            else if (cmd == 'E'){
                int src, dst;
                vector<int> vertex; // store edges info
                char c = 0; // read separators
                input >> c;
                while (input)
                {
                    if ((c >= '0') && (c <= '9'))
                    {
                        input.putback(c);
                        input >> src;
                        input >> c; //skip comma
                        input >> dst;
                        if (src < 0 || dst < 0 || src > vtxNum- 1 || dst > vtxNum - 1 || src == dst)
                            throw Exception("vertex index out of range");
                        else if (graph->exist(src, dst))
                            throw Exception("edge is already existing");
                        else {
                            vertex.push_back(src);
                            vertex.push_back(dst);
                        }
                    }
                    input >> c;
                }
                for (size_t i = 0; i < vertex.size() ; i += 2) {
                    graph->insert(vertex[i], vertex[i + 1]);
                    graph->insert(vertex[i + 1], vertex[i]);
                }
                vertex.clear();
            }

            //search path
            else {
                int src, dst;
                input >> src >> dst;
                if (src < 0 || dst < 0 || src > vtxNum- 1 || dst > vtxNum - 1 || src == dst)
                    throw Exception("vertex index out of range");
                if (!graph->path(src, dst))
                    throw Exception("no path founded");
            }
        }
        catch(Exception &exp){
            std::cerr << "Error: " << exp.what() << endl;
        }
    }
    delete(graph);
    exit(0) ;
}
