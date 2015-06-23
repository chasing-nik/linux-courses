import os


class FileReader:
    """This class reads data from file"""
    def __init__(self, fpath):
        self.fpath = fpath

    def readlines(self):
        """Reads file line by line and return list of lines"""
        if os.path.exists(self.fpath):
            with open(self.fpath, 'r') as f:
                lines = [line.strip() for line in f]
            return lines
        else:
            raise IOError('file not found')