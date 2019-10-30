//
// Created by Rongzhi on 2019/10/20.
//
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include "Error.h"
#include "Graph.h"

using namespace std;

#define DE_MAX_ST_NUM 10
#define DE_MAX_LINE_NUM 5
#define DE_MAX_WAIT_TIME 5
#define DE_COR_RANGE 20
#define DE_ATTEMPT_NUM 25

int getRandom(int min, int max) {
    //open /dev/urandom for reading
    ifstream urandom("/dev/urandom");

    //check if open failed
    if (urandom.fail())
        throw Exception("can not open /dev/urandom");

    //read a random int
    int randomData = 0;
    urandom.read((char*)&randomData, sizeof(int));

    //resize the number to required range
    randomData = abs(randomData) % (max - min + 1) + min;

    //close random stream
    urandom.close();

    return randomData;
}

bool isIntersect(Line l1, Line l2, bool diffSt) {
    int x1 = l1.src.x, x2 = l1.dst.x, x3 = l2.src.x, x4 = l2.dst.x,
        y1 = l1.src.y, y2 = l1.dst.y, y3 = l2.src.y, y4 = l2.dst.y;

    //for neibouring lines
    if (x2 == x3 && y2 == y3 && !diffSt) {
        if ((x2 - x1) * (y4 - y1) - (y2 - y1) * (x4 - x1)) {
            return false;
        }
        else if (((x2 - x1) * (x4 - x2) >= 0) && ((y2 - y1) * (y4 - y2)) >= 0) {
            return false;
        }
        return true;
    }

    //quick exclusion
    if ((max(x3, x4) < min(x1, x2)) ||
        (max(y3, y4) < min(y1, y2)) ||
        (max(x1, x2) < min(x3, x4)) ||
        (max(y1, y2) < min(y3, y4)))
        return false;

    //straddle method
    return !((((x1 - x3) * (y4 - y3) - (y1 - y3) * (x4 - x3)) *
              ((x2 - x3) * (y4 - y3) - (y2 - y3) * (x4 - x3))) > 0 ||
             (((x3 - x1) * (y2 - y1) - (y3 - y1) * (x2 - x1)) *
              ((x4 - x1) * (y2 - y1) - (y4 - y1) * (x2 - x1))) > 0);
}

bool isValid(Line _line, const vector<Line> &_thisSt, const vector<vector<Line>> &_allSt,
             bool &_haveIntersect, bool &_lastLine) {
    //zero-length
    if (_line.src.x == _line.dst.x && _line.src.y == _line.dst.y)
        return false;


    //intersect with line-segment from the same street
    for (const auto& l : _thisSt) {
        if (isIntersect(l, _line, false))
            return false;
    }

    //overlap with other streets
    for (const auto& s : _allSt) {
        for (const auto& l : s) {
            if ((_line.src.x == l.src.x && _line.src.y == l.src.y &&
                 _line.dst.x == l.dst.x && _line.dst.y == l.dst.y) ||
                (_line.src.x == l.dst.x && _line.src.y == l.dst.y &&
                 _line.dst.x == l.src.x && _line.dst.y == l.src.y))
                return false;
            if (isIntersect(l, _line, true))
                _haveIntersect = true;
        }
    }

    //no intersections acorss all streets
    return !(_lastLine && !_haveIntersect);

}

void genInput(int _maxStNum, int _maxLineNum, int _corRange, vector<string> &_stName) {
    //issue r commands to clear streets and clear vector
    for (const auto& st : _stName)
        cout << "r " << '"' << st << '"' << endl;
    _stName.clear();

    //generate inputs
    //randomly generate the number of streets and line-segments
    int stNum = getRandom(2, _maxStNum);
    vector<Line> thisSt;
    vector<vector<Line>> allSt;
    bool haveIntersect = false, lastLine;
    for (int j = 0; j < stNum; ++j) {
        //generate street name alphabetically
        string name;
        name.assign(j / 26 + 1, char(j % 26 + 65));
        name.append(" Street");
        _stName.push_back(name);

        //generate line-segments randomly
        thisSt.clear();
        int lineNum = getRandom(1, _maxLineNum);
        for (int k = 0; k < lineNum; ++k) {
            lastLine = ((j == stNum - 1) && (k == lineNum - 1));
            int count = DE_ATTEMPT_NUM;
            while (count) {
                Point src{};
                if (thisSt.empty()) {
                    src.x = getRandom(-_corRange, _corRange);
                    src.y = getRandom(-_corRange, _corRange);
                }
                else {
                    src.x = thisSt.back().dst.x;
                    src.y = thisSt.back().dst.y;
                }

                Point dst = {getRandom(-_corRange, _corRange),
                             getRandom(-_corRange, _corRange)};
                Line line = {src, dst};

                //check if valid
                if (isValid(line, thisSt, allSt, haveIntersect, lastLine)) {
                    thisSt.push_back(line);
                    break;
                }
                else
                    count -= 1;
            }
            if (!count) {
                string msg = "failed to generate valid input for " +
                             to_string(DE_ATTEMPT_NUM) + " simultaneous attemps";
                throw Exception(msg.c_str());
            }
        }
        allSt.push_back(thisSt);

        //issue a commands to stdout(a1)
        cout << "a " << '"' << name << '"';
        for (const auto& l : thisSt)
            printf(" (%d,%d)", l.src.x, l.src.y);
        printf(" (%d,%d)\n", thisSt.back().dst.x, thisSt.back().dst.y);
    }

    //issue a g command
    cout << 'g' << endl;
}

int main(int argc, char** argv) {
    int maxStNum = DE_MAX_ST_NUM,
        maxLineNum = DE_MAX_LINE_NUM,
        maxWaitTime = DE_MAX_WAIT_TIME,
        corRange = DE_COR_RANGE;
    vector<string> stName;
    try {
        //parse options and arguments
        // expected options are 's value', 'n value', 'l value' and 'c value'
        int cmd;
        while (( cmd = getopt(argc, argv, "s:n:l:c:")) != -1)
            switch (cmd) {
                case 's':
                    maxStNum = (int)strtol(optarg, nullptr, 10);
                    continue;
                case 'n':
                    maxLineNum = (int)strtol(optarg, nullptr, 10);
                    continue;
                case 'l':
                    maxWaitTime = (int)strtol(optarg, nullptr, 10);
                    continue;
                case 'c':
                    corRange = (int)strtol(optarg, nullptr, 10);
                    continue;
                case '?':
                    throw Exception("wrong option or argument");
                default:
                    break;
            }

        while (true) {
            genInput(maxStNum, maxLineNum, corRange, stName);

            //wait for generating the next input
            sleep(getRandom(5, maxWaitTime));
        }

    }
    catch(Exception &exp){
        cerr << "Error: " << exp.what() << endl;
    }
    exit(0);
}

