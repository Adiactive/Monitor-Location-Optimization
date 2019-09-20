"""
Rongzhi Gu
20855042

test example:
a "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)
a "King Street S" (4,2) (4,8)
a "Davenport Road" (1,4) (5,8)
g

9-18
Error1:line overlap
Error2:option input error (e.g. g with more args)
"""
import sys
import re
from Class import *


def command_parser(_line, _st_info):
    # option a for adding a street
    if _line[0] == 'a' and _line[1] == ' ':
        name = re.findall(r'\"(.*)\"', _line)
        coord = re.findall(r'\(.*?\)', _line)
        _st_info.add(name, coord)
        _st_info.check_street()
        # print name
        # print coord
        return
    # option g for generating graph
    if _line[0] == 'g' and _line[1] == ' ':
        pass

    # option r for removing a street
    if _line[0] == 'r' and _line[1] == ' ':
        pass

    # option c for changing street info
    if _line[0] == 'c' and _line[1] == ' ':
        pass
    # print _line

    # if not anyone above, return error meg
    print('Error: error input, try one of the options "a, c, r, g"')
    return


def main():
    # YOUR MAIN CODE GOES HERE
    st_info = StreetDatabase()
    while True:
        line = sys.stdin.readline()
        if line == '':
            break
        command_parser(line, st_info)
    # print 'Finished reading input'
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == '__main__':
    main()
