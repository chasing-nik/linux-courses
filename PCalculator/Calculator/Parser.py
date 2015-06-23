class Parser:
    """This class provides functions for math expressions parsing"""

    def __init__(self):
        tokens = []

    def parse_complex_function(self, instr):
        """Gets function consisting of letters and digits from beginning of string"""
        token = ''
        last = False
        for j, c in enumerate(instr):
            if not ord(c) in range(ord('a'), ord('z') + 1) and not c.isdigit():
                break
            else:
                token += c
                if j == len(instr) - 1:
                    last = True
        self.tokens.append(token)
        if last:
            return ''
        else:
            return instr[j:]

    def parse_base_function(self, instr):
        """Gets standart math operator from beginning of string"""
        if len(instr) > 1 and (instr[0:2] == '**' or instr[0:2] == '//'):
            self.tokens.append(instr[0:2])
            if len(instr) == 2:
                return ''
            else:
                return instr[2:]
        else:
            self.tokens.append(instr[0])
            if len(instr) == 1:
                return ''
            else:
                return instr[1:]

    def parse_number(self, instr):
        """Gets number from beginning of string"""
        token = ''
        last = False
        for j, c in enumerate(instr):
            if not c.isdigit() and c != '.':
                break
            else:
                token += c
                if j == len(instr) - 1:
                    last = True
        self.tokens.append(token)
        if last:
            return ''
        else:
            return instr[j:]

    def parse(self, instr):
        """Determines next token"""
        self.tokens = []
        while len(instr) > 0:
            if instr[0] == ' ':
                instr = instr[1:]
            elif instr[0].isdigit() or instr[0] == '.':
                instr = self.parse_number(instr)
            else:
                if ord(instr[0]) in range(ord('a'), ord('z') + 1):
                    instr = self.parse_complex_function(instr)
                else:
                    instr = self.parse_base_function(instr)
        return self.tokens