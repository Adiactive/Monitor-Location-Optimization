"""
Rongzhi Gu
20855042

test example:
a "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)
a "King Street S" (4,2) (4,8)
c "KING STREET S" (1,2)
a "Davenport Road" (1,4) (5,8)
r "kInG StrEet S"
a "Weber Street" 1,2,3,4,5,6
a "Weber Street" (1,2 (3,4) (5,6)
a "Weber Street" (1,2)(3,4) (5,6)
a"Weber Street" (1,2) (3,4) (5,6)
a "Weber Street"(1,2) (3,4) (5,6)
g

9-18
Error1:line overlap
Error2:option input error (e.g. g with more args)
"""
import sys
import re
from Class import *


def command_parser(_line):
    arg_list = _line.split('"')
    if (_line[0] == 'a' or _line[0] == 'c' or _line[0] == 'r' or _line[0] == 'g') and _line[1] == ' ':
        name = re.findall(r'\"(.*)\"', _line)
        coord = re.findall(r'\(.*?\)', _line)
        # check args for `a` and `c`
        if (_line[0] == 'a' or _line[0] == 'c') and _line[2] == '"' and len(name) == 1 \
                and arg_list[2][0] == ' ' and coord != []:
            for subs in name[0].split():
                if not subs.isalpha():
                    raise Exception("street name allows alphabetical and space characters only")

            for i in coord:
                if len(tuple(eval(i))) != 2:
                    raise Exception("wrong coordinates")

        # check args for `r`
        elif _line[0] == 'r' and _line[2] == '"' and len(name) == 1 \
                and len(arg_list[2].strip()) == 0:
            for subs in name[0].split():
                if not subs.isalpha():
                    raise Exception("street name allows alphabetical and space characters only")

        # check args for `g`
        elif _line[0] == 'g' and len(_line.strip()) == 1:
            pass

        else:
            raise Exception("wrong argument for option `%c`" % _line[0])
        return _line[0], name, coord
    else:
        raise Exception("command unknown, try one of the options `a, c, r, g`")


def main():
    st_data = StreetDatabase()
    while True:
        try:
            line = sys.stdin.readline()
            if line == '':
                break
            cmd = command_parser(line)

            # option a for adding a street
            if cmd[0] == 'a':
                st_data.add(cmd[1], cmd[2])

            # option c for adding a street
            elif cmd[0] == 'c':
                st_data.change(cmd[1], cmd[2])

            # option r for removing a street
            elif cmd[0] == 'r':
                st_data.remove(cmd[1])

            # option g for generating graph
            elif cmd[0] == 'g':
                pass

        except SyntaxError:
            sys.stderr.write("Error: wrong coordinates\n")
        except Exception as exp:
            sys.stderr.write("Error: " + str(exp) + '\n')

        st_data.check_street()
    # print 'Finished reading input'
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == '__main__':
    main()
