// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <regex>
#include "Graph.h"
using namespace std;

//for error handling
struct Exception : std::runtime_error {
    explicit Exception(const char *msg) : std::runtime_error(msg) {}
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

    // read from stdin until EOF
    while (!cin.eof()) {
        std::string line;
        getline(cin, line);
        Graph<int>* graph = NULL;
        if (line.empty()) {
            continue;
        }
        try {
            // read a line of input until EOL and store in a string
            // create an input stream based on the line to parse it
            std::istringstream input(line);
            char cmd = command_parser(input);
            if (cmd == 'V'){
                int vtxNum;
                input >> vtxNum;
                if (input.fail())
                    throw Exception("can not parse vertex number");
                graph = new Graph<int>(vtxNum);
            }
            else if (cmd == 'E'){
                regex reg("(\\d)+");
                smatch results;
                int src, dst, count = 0;
                char c;
                input >> c;
                while(input)
                {
                    if((c>='0')&&(c<='9'))
                    {
                        input.putback(c);
                        input >> src;
                        input >>c;
                        input >>dst;
                        cout << src << ' ' << dst << endl;
                    }
                    input >> c;
                }

//                string::const_iterator start(line.cbegin());
//                while ( regex_search(start, line.cend(), results, reg)) {
//                    count += 1;
//                    if (count == 1)
//                        int src = stoi(results[0].str());
//                    else {
//                        int dst = stoi(results[0].str());
//
//                    }
//                    start = results.suffix().first;
//                }
            }
        }
        catch(Exception &exp){
            std::cerr << "Error:" << exp.what() << endl;
        }
    }
    exit(0) ;
}
