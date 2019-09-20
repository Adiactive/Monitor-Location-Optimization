class StreetDatabase:
    # store street info as a dictionary
    st_info = {}

    def add(self, name, coord):
        if name[0] not in self.st_info:
            self.st_info[name[0]] = []
        else:
            print('Error: %s specified for a street that has been added. Try using option c to change it') % name
            return False
        for i in coord:
            self.st_info[name[0]].append(tuple(eval(i)))
        return True

    def check_street(self):
        print(self.st_info)
