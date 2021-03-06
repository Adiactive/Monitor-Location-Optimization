/*
 * Created by Rongzhi on 2019/11/13.
 * see detailed codes measuring running time
 * and approximate ratio of the three methods
 * in Time branch
 */
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <pthread.h>
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
    pthread_t threads[3];
    vector<int> results[3];//store running time for each thread, for output concern
    vector<int> edges; // store src and dst vertex index in case error input are detected

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

                struct ArgStruct args[3]{
                    {vtxNum, edges, &results[0]},
                    {vtxNum, edges, &results[1]},
                    {vtxNum, edges, &results[2]}
                };

                //CNF-SAT-VC
                if (pthread_create(&threads[0], nullptr, &CNF_SAT_VC, (void*)&args[0]))
                    throw Exception("cannot create thread of CNF-SAT-VC");

                //APPROX_VC_1
                if (pthread_create(&threads[1], nullptr, &APPROX_VC_1, (void*)&args[1]))
                    throw Exception("cannot create thread of APPROX_VC_1");

                //APPROX_VC_2
                if (pthread_create(&threads[2], nullptr, &APPROX_VC_2, (void*)&args[2]))
                    throw Exception("cannot create thread of APPROX_VC_2");

                for (auto &t : threads)
                    pthread_join(t, nullptr);

                //print results
                //CNF-SAT-VC
                cout << "CNF-SAT-VC: ";
                if (results[0][0] == IS_TIME_OUT)
                    cout << "timeout";
                else {
                    for (size_t i = 0; i < results[0].size(); ++i) {
                        cout << results[0][i];
                        if (i != results[0].size() - 1)
                            cout << ',';
                    }
                }
                cout << endl;

                //APPROX-VC-1
                cout << "APPROX-VC-1: ";
                for (size_t i = 0; i < results[1].size(); ++i) {
                    cout << results[1][i];
                    if (i != results[1].size() - 1)
                        cout << ',';
                }
                cout << endl;

                //APPROX-VC-2
                cout << "APPROX-VC-2: ";
                for (size_t i = 0; i < results[2].size(); ++i) {
                    cout << results[2][i];
                    if (i != results[2].size() - 1)
                        cout << ',';
                }
                cout << endl;

                //clear all the results
                for (auto & r : results) {
                    r.clear();
                }

                edges.clear();
            }
        }
        catch(Exception &exp){
            cerr << "Error: " << exp.what() << endl;
        }
    }
    return 0;
}

