"""
Rongzhi Gu
20855042

test input:
a "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)
a "King Street S" (4,2) (4,8)
a "Davenport Road" (1,4) (5,8)
g
c "Weber Street" (2,1) (2,2)
g
r "King Street S"
g
}

bug fixing: segment overlap
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
                if len(tuple(eval(i))) != 2 or ' ' in i:
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
    # quick exclusion
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

    x_alpha = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))
    x_beta = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4))
    x = x_alpha / x_beta

    y_alpha = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)
    y_beta = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
    y = y_alpha / y_beta

    return Point(x, y)


def graph_printer(_graph):
    # print vertices
    print('V = {')
    for i in range(len(_graph.vertex)):
        print("  {0}:  ({1:.2f},{2:.2f})".format(i + 1, _graph.vertex[i].x, _graph.vertex[i].y))
    print('}')
    # print edges
    print('E = {')
    for i in range(len(_graph.edge)):
        print("  <{0},{1}>,".format(_graph.vertex.index(_graph.edge[i][0]) + 1,
                                    _graph.vertex.index(_graph.edge[i][1]) + 1))
    print('}')

def graph_generator(_st_data):
    graph = Graph()
    lines = []
    intersections = {}
    # turn points to lines
    for i in _st_data.st_data:
        lines_per_st = []
        for j in range(len(_st_data.st_data[i]) - 1):
            lines_per_st.append(Line(
                Point(_st_data.st_data[i][j][0], _st_data.st_data[i][j][1]),
                Point(_st_data.st_data[i][j + 1][0], _st_data.st_data[i][j + 1][1])))
        lines.append(lines_per_st)

    # generate intersection points for each line in each street (end-point of segment included)
    for pre_st in lines:
        for pre_line in pre_st:
            for pos_st in lines[lines.index(pre_st) + 1:]:
                for pos_line in pos_st:
                    if is_intersecting(pre_line, pos_line):
                        intersect_point = intersect(pre_line, pos_line)
                        if pre_line not in intersections:
                            intersections[pre_line] = [pre_line.src, pre_line.dst]
                        if intersect_point not in intersections[pre_line]:
                            intersections[pre_line].append(intersect_point)
                        if pos_line not in intersections:
                            intersections[pos_line] = [pos_line.src, pos_line.dst]
                        if intersect_point not in intersections[pos_line]:
                            intersections[pos_line].append(intersect_point)

    # generate vertices and edges
    for point_list in intersections.values():
        point_list.sort(key=lambda x: (x.x, x.y))
        for index in range(len(point_list) - 1):
            graph.add_vertex(point_list[index])
            graph.add_edge(point_list[index], point_list[index + 1])
        graph.add_vertex(point_list[-1])

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
                graph_printer(graph)

        except SyntaxError:
            sys.stderr.write("Error: wrong coordinates\n")
        except Exception as exp:
            sys.stderr.write("Error: " + str(exp) + '\n')

        # st_data.check_street()
    # print 'Finished reading input'
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == '__main__':
    main()
