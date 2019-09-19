"""
Rongzhi Gu
20855042

test example:
a "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)

9-18
Error1:line overlap
"""
import sys
import re

# store street info as a dictionary
st = {}


def add_street(_name, _coord):
    global st
    if _name[0] not in st:
        st[_name[0]] = []
    else:
        print 'Error: %s specified for a street that has been added. Try using option c to change it' % _name
        return False
    for i in _coord:
        st[_name[0]].append(tuple(eval(i)))
    return True

def parse_line(_line):
    # _line = 'a "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)'
    if _line[0] == 'a':
        name = re.findall(r'\"(.*)\"', _line)
        coord = re.findall(r'\(.*?\)', _line)
        # print name
        # print coord
        if not add_street(name, coord):
            return
    # print _line
    return

def main():
    ### YOUR MAIN CODE GOES HERE
    global st
    ### sample code to read from stdin.
    ### make sure to remove all spurious print statements as required
    ### by the assignment
    while True:
        line = sys.stdin.readline()
        if line == '':
            break
        parse_line(line)
        print st
    # print 'Finished reading input'
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == '__main__':
    main()
