#include <iostream>
#include <unistd.h>
#include <csignal>
#include <vector>
#include "Error.h"
#include <fcntl.h>
#include <sys/wait.h>
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

void runA2() {
    //set argument for ece650-a2
    char* argv[] = {
            (char*)"./ece650-a2",
            nullptr
    };

    execvp("./ece650-a2", argv);
    throw Exception("can not run ece650-a2");
}

int main (int argc, char **argv) {
    vector<pid_t> kids;
    pid_t childPid;

    try {
        //check arguments
        int cmd;
        while ((cmd = getopt(argc, argv, "s:n:l:c:")) != -1)
            switch (cmd) {
                case 's':
                    if ((int)strtol(optarg, nullptr, 10) < 2)
                        throw Exception("-s option requires an integer >= 2");
                    continue;
                case 'n':
                    if ((int)strtol(optarg, nullptr, 10) < 1)
                        throw Exception("-n option requires an integer >= 1");
                    continue;
                case 'l':
                    if ((int)strtol(optarg, nullptr, 10) < 5)
                        throw Exception("-l option requires an integer >= 5");
                    continue;
                case 'c':
                    if ((int)strtol(optarg, nullptr, 10) < 1)
                        throw Exception("-c option requires an integer >= 1");
                    continue;
                case '?':
                    throw Exception("wrong option or argument");
                default:
                    break;
            }

        //create pipes
        int rToA1[2], a1ToA2[2];
        //pipe for rgen and a1
        pipe(rToA1);
        //pipe for a1 and a2
        pipe(a1ToA2);

        //create a process for random input generator
        childPid = fork();
        if (childPid == 0) {
            //redirect stdout to write end of rToA1
            dup2(rToA1[1], STDOUT_FILENO);
            close(rToA1[0]);
            close(rToA1[1]);
            close(a1ToA2[0]);
            close(a1ToA2[1]);
            //run random generator
            runRdmGen(argv);
        }
        else if (childPid < 0)
            throw Exception("can not fork for rgen");

        kids.push_back(childPid);

        //create a process for a1
        childPid = fork();
        if (childPid == 0) {
            //redirect stdin to read end of rToA1
            dup2(rToA1[0], STDIN_FILENO);
            //redirect stdout to write end of a1ToA2
            dup2(a1ToA2[1], STDOUT_FILENO);
            close(rToA1[0]);
            close(rToA1[1]);
            close(a1ToA2[0]);
            close(a1ToA2[1]);
            //run a1
            runA1();
        }
        else if (childPid < 0)
            throw Exception("can not fork for a1");

        kids.push_back(childPid);

        //create a process for a2
        childPid = fork();
        if (childPid == 0) {
            //redirect stdin to read end of a1ToA2
            dup2(a1ToA2[0], STDIN_FILENO);
            close(a1ToA2[0]);
            close(a1ToA2[1]);
            close(rToA1[0]);
            close(rToA1[1]);

            //run a2
            runA2();
        }
        else if (childPid < 0)
            throw Exception("can not fork for a2");

        kids.push_back(childPid);

        //read s commands from keyboard
        //redirect stdout to write end of a1ToA2
        dup2(a1ToA2[1], STDOUT_FILENO);
        close(rToA1[0]);
        close(rToA1[1]);
        close(a1ToA2[0]);
        close(a1ToA2[1]);

        while (!cin.eof()) {
            // read a line of input until EOF
            string line;
            getline(std::cin, line);
            if (line.empty())
                if (cin.eof())
                    continue;
            cout << line << endl;
        }

        for (pid_t k : kids) {
            int status;
            kill(k, SIGTERM);
            waitpid(k, &status, 0);
        }

    }
    catch(Exception &exp){
        cerr << "Error: " << exp.what() << endl;
    }

    return 0;
}
