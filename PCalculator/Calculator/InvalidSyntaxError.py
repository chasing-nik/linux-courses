class InvalidSyntaxError(Exception):
    """This error is raised when arguments or brackets are missing"""
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return 'Invalid syntax: {0}'.format(self.value)