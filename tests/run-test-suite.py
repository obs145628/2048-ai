import os
import sys


import shell
from json_ts_reader import JsonTsReader
                
if len(sys.argv) != 2:
    print('Usage: python test-suite.py [json-tests-file]', file = sys.stderr)
    sys.exit(1)

path = shell.parse_val(sys.argv[1])
tests = JsonTsReader(path)
ts = tests.ts

ts.out_err = open(os.path.join(os.path.dirname(path), 'errors.log'), 'w')

ts.run()
