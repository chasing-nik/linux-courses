import math
from FunctionParseError import FunctionParseError
from NumberParseError import NumberParseError
from InvalidSyntaxError import InvalidSyntaxError


class Calculator:
    """This class calculates expression from list of tokens"""

    def __init__(self):
        self.functions = []
        self.operands = []

    def is_base_function(self, function):
        """Checks if function is '+', '-', '*', '/', '**', '^', '//', '%'"""
        return function in ('+', '-', '*', '/', '**', '^', '//', '%')


    def is_constant(self, function):
        """Checks if token is constant"""
        return function in ('e', 'pi')

    def append_constant(self, operands, constant):
        """Appends constant to operands"""
        if constant == 'pi':
            operands.append(math.pi)
        elif constant == 'e':
            operands.append(math.e)

    def right_associative(self, function):
        """Checks if function is right associative"""
        return function in ('u+', 'u-', '**', '^')


    def is_function(self, function):
        """Checks if token is function"""
        return self.is_base_function(function) or function in (
            'ceil', 'copysign', 'fabs', 'factorial', 'floor', 'fmod', 'frexp', 'fsum', 'isinf', 'isnan', 'ldexp',
            'modf',
            'trunc', 'exp', 'expm1', 'log', 'log1p', 'log10', 'pow', 'acos', 'asin', 'atan', 'atan2', 'cos', 'hypot',
            'sin',
            'tan', 'acosh', 'asinh', 'atanh', 'cosh', 'sinh', 'tanh', 'erf', 'erfc', 'gamma', 'lgamma', 'sqrt', 'abs',
            'max', 'min', 'sum')


    def priority(self, function):
        """Returns priority of function"""
        if not self.is_base_function(function) and self.is_function(function):
            return 5
        elif function in ('**', '^'):
            return 4
        elif function in ('u-', 'u+'):
            return 3
        elif function in ('*', '/', '//', '%'):
            return 2
        elif function in ('+', '-'):
            return 1
        else:
            return 0


    def perform_func(self, operands, function, argcount=0):
        """Perform function and puts result to operand list"""
        if function in ('u-', 'u+') or argcount == 1:
            if len(operands) < 1:
                raise FunctionParseError('Arguments are missing ' + function)
            argument = operands[-1]
            operands.pop()
            if function == 'u-':
                operands.append(-argument)
            elif function == 'u+':
                operands.append(argument)
            elif function == 'ceil':
                operands.append(math.ceil(argument))
            elif function == 'sqrt':
                operands.append(math.pow(argument, 0.5))
            elif function == 'abs':
                operands.append(abs(argument))
            elif function == 'fabs':
                operands.append(math.fabs(argument))
            elif function == 'factorial':
                operands.append(math.factorial(argument))
            elif function == 'floor':
                operands.append(math.floor(argument))
            elif function == 'frexp':
                operands.append(math.frexp(argument))
            elif function == 'isinf':
                operands.append(math.isinf(argument))
            elif function == 'isnan':
                operands.append(math.isnan(argument))
            elif function == 'modf':
                operands.append(math.modf(argument))
            elif function == 'trunc':
                operands.append(math.trunc(argument))
            elif function == 'exp':
                operands.append(math.exp(argument))
            elif function == 'expm1':
                operands.append(math.expm1(argument))
            elif function == 'log':
                operands.append(math.log(argument))
            elif function == 'log1p':
                operands.append(math.log1p(argument))
            elif function == 'log10':
                operands.append(math.log10(argument))
            elif function == 'sqrt':
                operands.append(math.sqrt(argument))
            elif function == 'acos':
                operands.append(math.acos(argument))
            elif function == 'asin':
                operands.append(math.asin(argument))
            elif function == 'atan':
                operands.append(math.atan(argument))
            elif function == 'cos':
                operands.append(math.cos(argument))
            elif function == 'sin':
                operands.append(math.sin(argument))
            elif function == 'tan':
                operands.append(math.tan(argument))
            elif function == 'degrees':
                operands.append(math.degrees(argument))
            elif function == 'radians':
                operands.append(math.radians(argument))
            elif function == 'acosh':
                operands.append(math.acosh(argument))
            elif function == 'asinh':
                operands.append(math.asinh(argument))
            elif function == 'atanh':
                operands.append(math.atanh(argument))
            elif function == 'cosh':
                operands.append(math.cosh(argument))
            elif function == 'sinh':
                operands.append(math.sinh(argument))
            elif function == 'tanh':
                operands.append(math.tanh(argument))
            elif function == 'erf':
                operands.append(math.erf(argument))
            elif function == 'erfc':
                operands.append(math.erfc(argument))
            elif function == 'gamma':
                operands.append(math.gamma(argument))
            elif function == 'lgamma':
                operands.append(math.lgamma(argument))
            else:
                raise FunctionParseError('Wrong argument count in ' + function)
        elif argcount <= 2:
            if len(operands) < 2:
                raise FunctionParseError('Arguments are missing ' + function)
            rightoper = operands[-1]
            operands.pop()
            leftoper = operands[-1]
            operands.pop()
            if function == '+':
                operands.append(leftoper + rightoper)
            elif function == '-':
                operands.append(leftoper - rightoper)
            elif function == '*':
                operands.append(leftoper * rightoper)
            elif function == '/':
                operands.append(leftoper / rightoper)
            elif function == '**':
                operands.append(math.pow(leftoper, rightoper))
            elif function == '^':
                operands.append(math.pow(leftoper, rightoper))
            elif function == '//':
                operands.append(leftoper // rightoper)
            elif function == '%':
                operands.append(leftoper % rightoper)
            elif function == 'copysign':
                operands.append(leftoper, rightoper)
            elif function == 'fmod':
                operands.append(math.fmod(leftoper, rightoper))
            elif function == 'ldexp':
                operands.append(math.ldexp(leftoper, rightoper))
            elif function == 'pow':
                operands.append(math.pow(leftoper, rightoper))
            elif function == 'atan2':
                operands.append(math.atan2(leftoper, rightoper))
            elif function == 'log':
                operands.append(math.log(leftoper, rightoper))
            elif function == 'hypot':
                operands.append(math.hypot(leftoper, rightoper))
            else:
                raise FunctionParseError('Wrong argument count in ' + function)
        else:
            if len(operands) < argcount:
                raise FunctionParseError('Arguments are missing ' + function)
            if function == 'fsum':
                result = math.fsum(operands[-argcount:])
            elif function == 'max':
                result = max(operands[-argcount:])
            elif function == 'min':
                result = min(operands[-argcount:])
            elif function == 'sum':
                result = sum(operands[-argcount:])
            else:
                raise FunctionParseError('Wrong argument count in ' + function)
            operands[-argcount:] = []
            operands.append(result)


    def calculate(self, tokens):
        """Defines sequence of math operations and calculates expression"""
        may_unary = True
        operands = []
        functions = []
        curtoken = ''
        while len(tokens) > 0:
            curtoken = tokens.pop(0)
            if curtoken == '(':
                functions.append(curtoken)
                may_unary = True
            elif curtoken == ',':
                while len(functions) > 0 and functions[-1] != '(' and functions[-1] != ',':
                    self.perform_func(operands, functions[-1])
                    functions.pop()
                if len(functions) == 0:
                    raise FunctionParseError('No bracket found')
                functions.append(curtoken)
                may_unary = True
            elif curtoken == ')':
                if len(tokens) > 0 and tokens[0] == '(':
                    tokens.insert(0, '*')
                while len(functions) > 0 and functions[-1] != '(' and functions[-1] != ',':
                    self.perform_func(operands, functions[-1])
                    functions.pop()
                if len(functions) == 0:
                    raise FunctionParseError('No bracket found')
                argcount = 1
                while len(functions) > 0 and functions[-1] != '(':
                    argcount += 1
                    functions.pop()
                if len(functions) == 0:
                    raise FunctionParseError('No bracket found')
                functions.pop()
                if len(functions) > 0 and not self.is_base_function(functions[-1]) and self.is_function(functions[-1]):
                    self.perform_func(operands, functions[-1], argcount)
                    functions.pop()
                may_unary = False
            elif ord(curtoken[0]) in range(ord('a'), ord('z') + 1) or self.is_base_function(curtoken):
                if self.is_constant(curtoken):
                    self.append_constant(operands, curtoken)
                    may_unary = False
                    continue
                elif not self.is_base_function(curtoken):
                    if not self.is_function(curtoken):
                        raise FunctionParseError(curtoken)
                    else:
                        if (len(tokens) > 0 and tokens[0] != '(') or len(tokens) == 0:
                            raise FunctionParseError('Missing brackets ' + curtoken)
                if may_unary and curtoken in ('-', '+'):
                    curtoken = 'u' + curtoken
                while len(functions) > 0 and (
                                self.right_associative(curtoken) and self.priority(functions[-1]) > self.priority(
                                    curtoken)
                        or (self.right_associative(curtoken) == False and self.priority(functions[-1]) >= self.priority(
                                curtoken))):
                    self.perform_func(operands, functions[-1])
                    functions.pop()
                functions.append(curtoken)
                may_unary = True
            else:
                dotcount = curtoken.count('.')
                if dotcount > 1:
                    raise NumberParseError(curtoken)
                else:
                    if dotcount == 1:
                        operand = float(curtoken)
                    else:
                        operand = int(curtoken)
                    operands.append(operand)
                    if len(tokens) > 0 and (
                                        self.is_function(tokens[0]) and not self.is_base_function(tokens[0]) or tokens[
                                    0] == '(' or self.is_constant(tokens[0])):
                        tokens.insert(0, '*')
                may_unary = False
        while len(functions) > 0:
            self.perform_func(operands, functions[-1])
            functions.pop()
        if len(operands) > 1:
            raise InvalidSyntaxError('Missing functions')
        if len(operands) == 0:
            raise NumberParseError('No number found')
        else:
            return operands[-1]




























