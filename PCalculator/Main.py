import argparse
from Calculator.Calculator import Calculator
from Calculator.Parser import Parser
from Calculator.InvalidSyntaxError import InvalidSyntaxError
from Calculator.NumberParseError import NumberParseError
from Calculator.FunctionParseError import FunctionParseError
import FileIO.FileReader
import sys

argparser = argparse.ArgumentParser()
argparser.add_argument("-f", help="file path")
args = argparser.parse_args()
if not args.f:
    print 'No file option'
    sys.exit()
freader = FileIO.FileReader.FileReader(args.f)
try:
    expressions = freader.readlines()
except IOError:
    print 'File doesnt exist'
    sys.exit()
myparser = Parser()
mycalc = Calculator()
i = 1
for expression in expressions:
    tokens = myparser.parse(expression)
    try:
        result = mycalc.calculate(tokens)
    except FunctionParseError as exc:
        print '{0}){1}'.format(i, exc)
    except NumberParseError as exc:
        print '{0}){1}'.format(i, exc)
    except InvalidSyntaxError as exc:
        print '{0}){1}'.format(i, exc)
    except FloatingPointError:
        print '{0}){1}'.format(i, 'Floating point error')
    except ZeroDivisionError:
        print '{0}){1}'.format(i, 'Zero division')
    except OverflowError:
        print '{0}){1}'.format(i, 'Overflow')
    except ValueError:
        print '{0}){1}'.format(i, 'ValueError')
    else:
        print '{0}){1}'.format(i, result)
    i += 1
