#include <iostream>
#include <unistd.h>
#include <csignal>
#include <vector>
#include "Error.h"
using namespace std;


void runRdmGen(char **argv) {
    //set argument for rgen
    argv[0] = (char*)"./rgen";

    //run rgen
    execvp("./rgen", argv);
    throw Exception("can not run rgen");
}

void runA1() {
    //set argument for ece650-a1.py
    char* argv[] = {
            (char*)"python",
            (char*)"ece650-a1.py",
            nullptr
    };

    execvp("python", argv);
    throw Exception("can not run ece650-a1.py");
}

int main (int argc, char **argv) {
    vector<pid_t> kids;
    pid_t childPid;

    try {
        //create a pipe for rgen and a1
        int rToA1[2];
        pipe(rToA1);
        //create a process for random input generator
        childPid = fork();
        if (childPid == 0) {
            //redirect stdout to write end of rToA1
            dup2(rToA1[1], STDOUT_FILENO);
            close(rToA1[0]);
            close(rToA1[1]);

            //run random generator
            runRdmGen(argv);
        }
        else if (childPid < 0)
            throw Exception("can not fork");

        kids.push_back(childPid);

        //create a pipe for a1 and a2
        int a1ToA2[2];
        pipe(a1ToA2);
        //create a process for a1
        childPid = fork();
        if (childPid == 0) {
            //redirect stdin to read end of rToA1
            dup2(rToA1[0], STDIN_FILENO);
            //redirect stdin to read end of a1ToA2
            //dup2(a1ToA2[1], STDOUT_FILENO);
            close(rToA1[0]);
            close(rToA1[1]);
            close(a1ToA2[0]);
            close(a1ToA2[1]);

            //run a1
            runA1();
        }
        else if (childPid < 0)
            throw Exception("can not fork");

        while (!std::cin.eof()) {
            // read a line of input until EOL and store in a string
            std::string line;
            std::getline(std::cin, line);
            if (line.size () > 0)
                std::cout << "[a3]: " << line << std::endl;
        }
    }
    catch(Exception &exp){
        cerr << "Error: " << exp.what() << endl;
    }

    return 0;
}
