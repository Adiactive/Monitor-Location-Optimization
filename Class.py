class StreetDatabase(object):
    def __init__(self):
        # store street info in a dictionary
        self.st_data = {}

    def add(self, name, coord):
        if name[0].lower() not in [current_st.lower() for current_st in self.st_data]:
            self.st_data[name[0]] = []
            for i in coord:
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

    def check_street(self):
        print(self.st_data)

