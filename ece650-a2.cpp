// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

char command_parser(istringstream &input) {
    char opt;
    input >> opt;
    if (input.fail() || (opt != 'V' && opt != 'E' && opt != 's')) {
        throw string("unknown cmd option");
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
    // read from stdin until EOF
    while (!std::cin.eof()) {
        try {
            // read a line of input until EOL and store in a string
            std::string line;
            std::getline(std::cin, line);

            // create an input stream based on the line to parse it
            std::istringstream input(line);
            char cmd = command_parser(input);
            if (cmd == 'V'){

            }
            else if (cmd == 'E'){

            }
            else {

            }
        }
        catch(std::string exp){
            std::cerr << "Error:" << exp << std::endl;
        }


    }
    return 0;
}
