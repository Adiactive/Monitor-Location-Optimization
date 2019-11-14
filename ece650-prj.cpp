#include <iostream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <thread>
#include "vtxcover.h"
using namespace std;

//for error handling
struct Exception : std::runtime_error {
    explicit Exception(const char *msg) : runtime_error(msg) {}
};

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
                if (input.fail() || vtxNum <= 0)
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


                //CNF-SAT-VC
                thread t1(CNF_SAT_VC, vtxNum, edges);
                t1.join();
                //CNF_SAT_VC(vtxNum, edges);

                //APPROX_VC_1
                thread t2(APPROX_VC_1,vtxNum, edges);
                t2.join();
                //APPROX_VC_1(vtxNum, edges);

                //APPROX_VC_2
                thread t3(APPROX_VC_2,vtxNum, edges);
                t3.join();
                //APPROX_VC_2(vtxNum, edges);

//                t1.join();
//                t2.join();
//                t3.join();
                edges.clear();
            }
        }
        catch(Exception &exp){
            cerr << "Error: " << exp.what() << endl;
        }
    }
    return 0;
}

