// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
using namespace std;

//for error handling
class Exception: public exception
{
public:
    explicit Exception(const char* message): _msg(message) {}
    explicit Exception(std::string  message): _msg(std::move(message)) {}

    ~Exception() noexcept override= default;

    const char* what() const noexcept override{
        return _msg.c_str();
    }

protected:
    string _msg;
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
    // read from stdin until EOF
    while (!std::cin.eof()) {
        try {
            // read a line of input until EOL and store in a string
            std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof())
                break;

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

            }
            else {

            }
            if (input.eof())
                continue;
        }
        catch(Exception &exp){
            std::cerr << "Error:" << exp.what() << endl;
        }


    }
    return 0;
}
