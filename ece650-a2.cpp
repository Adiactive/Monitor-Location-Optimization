// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
using namespace std;

//for error handling
struct Exception : std::runtime_error {
    explicit Exception(const char *msg) : std::runtime_error(msg) {}
};

char command_parser(istringstream &input) {
    char opt;
    input >> opt;
    if (input.fail() || (opt != 'V' && opt != 'E' && opt != 's')) {
        throw Exception("unknown command option, expect `V, E, s`");
    }
    // parse command option
    return opt;
}
//    while (!input.eof()) {
//        unsigned num;
//        // parse an integer
//        input >> num;
//        if (input.fail()) {
//            std::cerr << "Error parsing a number\n";
//            break;
//        }
//
//        // if eof bail out
//        if (input.eof())
//            break;
//
//        // read a character
//        // Note that whitespace is ignored
//        char separator;
//        input >> separator;
//
//        // if error parsing, or if the character is not a comma
//        if (input.fail()) {
//            std::cerr << "Error parsing separator\n";
//            break;
//        }
//    }

int main(int argc, char** argv) {
    std::string line;
    //std::getline(std::cin, line);
    // read from stdin until EOF
    while (!getline(cin, line).eof()) {
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
                char separator;
                unsigned vertex;
                vector<unsigned> edge;
                while (!input.eof()) {
                    input >> vertex;
                    if (input.fail()) {
                        input >> separator;
                        cout << separator;
                    }
                    else
                        edge.push_back(vertex);
                }
                for (unsigned x : edge) {
                    cout << x;
                }

            }
            else {

            }
        }
        catch(Exception &exp){
            std::cerr << "Error:" << exp.what() << endl;
        }
    }
    exit(0) ;
}
