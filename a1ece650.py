"""
Rongzhi Gu
20855042

test example:
a "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)
a "King Street S" (4,2) (4,8)
a "Davenport Road" (1,4) (5,8)
g
"""
import sys
import re
from Class import *


def command_parser(_line):
    arg_list = _line.split('"')
    if ((_line[0] == 'a' or _line[0] == 'c' or _line[0] == 'r') and _line[1] == ' ') or \
            _line[0] == 'g' and len(_line.strip()) == 1:
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
        elif _line[0] == 'g':
            pass

        else:
            raise Exception("wrong argument for option `%c`" % _line[0])
        return _line[0], name, coord
    else:
        raise Exception("command unknown, try one of the options `a, c, r, g`")


def is_intersecting(l1, l2):
    # quick rejection
    if (max(l2.src.x, l2.dst.x) < min(l1.src.x, l1.dst.x)) or \
            (max(l2.src.y, l2.dst.y) < min(l1.src.y, l1.dst.y)) or \
            (max(l1.src.x, l1.dst.x) < min(l2.src.x, l2.dst.x)) or \
            (max(l1.src.y, l1.dst.y) < min(l2.src.y, l2.dst.y)):
        return False

    # straddle method
    if ((((l1.src.x - l2.src.x) * (l2.dst.y - l2.src.y) - (l1.src.y - l2.src.y) * (l2.dst.x - l2.src.x)) *
         ((l1.dst.x - l2.src.x) * (l2.dst.y - l2.src.y) - (l1.dst.y - l2.src.y) * (l2.dst.x - l2.src.x))) > 0 or
            (((l2.src.x - l1.src.x) * (l1.dst.y - l1.src.y) - (l2.src.y - l1.src.y) * (l1.dst.x - l1.src.x)) *
             ((l2.dst.x - l1.src.x) * (l1.dst.y - l1.src.y) - (l2.dst.y - l1.src.y) * (l1.dst.x - l1.src.x))) > 0):
        return False
    return True

def intersect(l1, l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    x_num = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))
    x_den = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
    x = x_num / x_den

    y_num = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
    y_den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
    y = y_num / y_den

    return Point(x, y)


def graph_generator(_st_data):
    graph = Graph()
    lines = []
    # turn points to lines
    for i in _st_data.st_data:
        lines_per_st = []
        for j in range(len(_st_data.st_data[i]) - 1):
            lines_per_st.append(Line(
                Point(_st_data.st_data[i][j][0], _st_data.st_data[i][j][1]),
                Point(_st_data.st_data[i][j + 1][0], _st_data.st_data[i][j + 1][1])))
        lines.append(lines_per_st)

    """
        for i in range(len(lines)-1):
        for dst_st in range(i + 1, len(lines)):
            for src_line in 
    """
    for pre_st in lines:
        for pre_line in pre_st:
            for pos_st in lines[lines.index(pre_st) + 1:]:
                for pos_line in pos_st:
                    if is_intersecting(pre_line, pos_line):
                        print(pre_line, pos_line, intersect(pre_line, pos_line))

    return graph


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
                graph = graph_generator(st_data)

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
