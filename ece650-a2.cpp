// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <regex>
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
        if (line.empty()) {
            continue;
        }
        try {
            // read a line of input until EOL and store in a string
            // create an input stream based on the line to parse it
            std::istringstream input(line);
            char cmd = command_parser(input);
            if (cmd == 'V'){
                unsigned vertexNum;
                input >> vertexNum;
                if (input.fail())
                    throw Exception("can not parse vertex number");
            }
            else if (cmd == 'E'){
                regex reg("(\\d)+");
                smatch results;
                vector<int> edges;
                string::const_iterator search_start(line.cbegin());
                while ( regex_search(search_start, line.cend(), results, reg)) {
                    edges.push_back(stoi(results[0].str()));
                    search_start = results.suffix().first;
                }
                for (int x : edges) {
                    cout << x << endl;
                }

            }
        }
        catch(Exception &exp){
            std::cerr << "Error:" << exp.what() << endl;
        }
    }
    exit(0) ;
}
