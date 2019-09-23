class StreetDatabase(object):
    def __init__(self):
        # store street info in a dictionary
        self.st_data = {}

    def add(self, name, coord):
        if name[0].lower() not in [current_st.lower() for current_st in self.st_data]:
            self.st_data[name[0]] = []
            for i in coord:
                if tuple(eval(i)) in self.st_data[name[0]]:
                    del self.st_data[name[0]]
                    raise Exception('coordinates overlap')
                self.st_data[name[0]].append(tuple(eval(i)))
        else:
            raise Exception('`a` specified for a street that has been added. Try option `c` to change it')

    def change(self, name, coord):
        if name[0].lower() not in [current_st.lower() for current_st in self.st_data]:
            raise Exception('`c` specified for a street that has not been added. Try option `a` to add it')
        else:
            for current_st in self.st_data:
                if name[0].lower() == current_st.lower():
                    self.st_data[current_st] = []
                    for i in coord:
                        self.st_data[current_st].append(tuple(eval(i)))
                    break

    def remove(self, name):
        if name[0].lower() not in [current_st.lower() for current_st in self.st_data]:
            raise Exception('`r` specified for a street that has not been added. Try option `a` to add it')
        else:
            for current_st in self.st_data:
                if name[0].lower() == current_st.lower():
                    del self.st_data[current_st]
                    break

    def check_street(self):
        print(self.st_data)

class Point(object):
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __repr__(self):
        return '({0:.2f},{1:.2f})'.format(self.x, self.y)

    def __str__(self):
        return repr(self)

    def __eq__(self, other):
        return (self.x, self.y) == (other.x, other.y)


class Line(object):
    def __init__(self, p1, p2):
        self.src = p1
        self.dst = p2

    def __repr__(self):
        return repr(self.src) + ' --> ' + repr(self.dst)

    def __hash__(self):
        return hash((self.src, self.dst))

    def __eq__(self, other):
        return (self.src, self.dst) == (other.src, other.dst)

class Graph(object):
    def __init__(self):
        self.vertex = []
        self.edge = []

    def add_vertex(self, p):
        if p not in self.vertex:
            self.vertex.append(p)

    def add_edge(self, p1, p2):
        self.edge.append((p1, p2))




