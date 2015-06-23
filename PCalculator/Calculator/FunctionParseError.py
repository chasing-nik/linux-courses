class FunctionParseError(Exception):
    """This error is raised when such function doesnt exist"""
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return 'Error while parsing {0}'.format(self.value)