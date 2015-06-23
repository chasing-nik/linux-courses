class NumberParseError(Exception):
    """This error is raised when token cannot be convert to number"""
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return 'Wrong number: {0}'.format(self.value)