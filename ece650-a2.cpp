// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <regex>
#include "Graph.h"
using namespace std;

#define default_w 1

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
    Graph<int>* graph = nullptr;
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
            if (cmd == 'V'){
                input >> vtxNum;
                if (input.fail())
                    throw Exception("can not parse vertex number");
                if (graph)
                    delete graph;
                graph = new Graph<int>(vtxNum);
            }
            else if (cmd == 'E'){
                int src, dst;
                vector<int> vertex;
                char c;
                input >> c;
                while (input)
                {
                    if ((c >= '0') && (c <= '9'))
                    {
                        input.putback(c);
                        input >> src;
                        input >> c; //skip comma
                        input >> dst;
                        if (src < 0 || dst < 0 || src >  vtxNum- 1 || dst > vtxNum - 1 || src == dst)
                            throw Exception("vertex index out of range");
                        else if (graph->exist(src, dst))
                            throw Exception("edge has already existed");
                        else {
                            vertex.push_back(src);
                            vertex.push_back(dst);
                        }
                    }
                    input >> c;
                }
                for (size_t i = 0; i < vertex.size() ; i += 2) {
                    graph->insert(vertex[i], vertex[i + 1], default_w);
                    graph->insert(vertex[i + 1], vertex[i], default_w);
                }
                graph->show(cout);
                vertex.clear();
            }
            else {
                
            }
        }
        catch(Exception &exp){
            std::cerr << "Error:" << exp.what() << endl;
        }
    }
    if (graph)
        delete(graph);
    graph = nullptr;
    exit(0) ;
}
