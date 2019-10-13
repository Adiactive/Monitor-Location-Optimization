#include <iostream>
#include <sstream>
#include <vector>
#include "Graph.h"
using namespace std;

//for error handling
struct Exception : runtime_error {
    explicit Exception(const char *msg) : runtime_error(msg) {}
};

// parse command option
char ParseCmd(istringstream &input) {
    char cmd;
    input >> cmd;
    if (input.fail() || (cmd != 'V' && cmd != 'E' && cmd != 's')) {
        throw Exception("unknown command option, expect `V, E, s`");
    }
    return cmd;
}

int main(int argc, char** argv) {
    Graph* graph = nullptr;
    int vtxNum = 0;
    // read from stdin until EOF
    while (!cin.eof()) {
        string line;
        getline(cin, line);
        if (line.empty()) {
            continue;
        }
        try {
            // read a line of input until EOL and store in a string
            // create an input stream based on the line to parse it
            istringstream input(line);
            char cmd = ParseCmd(input);

            //initialize graph
            if (cmd == 'V'){
                input >> vtxNum;
                if (input.fail() || vtxNum <= 0)
                    throw Exception("wrong vertex number");
                delete graph;
                graph = new Graph(vtxNum);
            }

            //initialize edge
            else if (cmd == 'E'){
                int src, dst;
                vector<int> vertex; // store src and dst vertex index in case error input are detected
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
                        if (src < 0 || dst < 0 || src > vtxNum- 1 || dst > vtxNum - 1)
                            throw Exception("vertex index out of range");
                        else if (graph->Exist(src, dst))
                            throw Exception("add duplicate edge(s)");
                        else {
                            vertex.push_back(src);
                            vertex.push_back(dst);
                        }
                    }
                    input >> c;
                }
                for (size_t i = 0; i < vertex.size() ; i += 2) {
                    graph->Insert(vertex[i], vertex[i + 1]);
                    graph->Insert(vertex[i + 1], vertex[i]);
                }
                vertex.clear();
            }

            //search path
            else {
                int src, dst;
                input >> src >> dst;
                if (src < 0 || dst < 0 || src > vtxNum- 1 || dst > vtxNum - 1)
                    throw Exception("vertex index out of range");
                if (!graph->Path(src, dst))
                    throw Exception("no path Exist");
            }
        }
        catch(Exception &exp){
            cout << "Error: " << exp.what() << endl;
        }
    }
    delete(graph);
    exit(0) ;
}
