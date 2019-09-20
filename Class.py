class StreetDatabase(object):
    def __init__(self):
        # store street info in a dictionary
        self.st_info = {}

    def add(self, name, coord):
        if name[0] not in self.st_info:
            self.st_info[name[0]] = []
        else:
            print('Error: "%s" specified for a street that has been added. Try option c to change it' % name[0])
            return False
        for i in coord:
            self.st_info[name[0]].append(tuple(eval(i)))
        return True

    def check_street(self):
        print(self.st_info)
