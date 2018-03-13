
from json_ts_builder import JsonTsBuilder

NTESTS = 8
IMPLS = ['world']
PY_IMPLS = ['world']

ts = JsonTsBuilder()

for impl in IMPLS:
    for i in range(1, NTESTS + 1):
        ts.add_test('gridtest', impl + '_game' + str(i),                    
                    cmd = ['{BUILD_DIR}/2048-ai', 'gridtest', impl,
                           '{ROOT_DIR}/tests/games/game' + str(i) + '_moves'],
                    code = 0,
                    stdout_file = '{ROOT_DIR}/tests/games/game' + str(i) + '_gridtest_ref')

for impl in PY_IMPLS:
    for i in range(1, NTESTS + 1):
        ts.add_test('python_gridtest', impl + '_game' + str(i),
                    cmd = ['python', '{ROOT_DIR}/src_py/main.py', 'gridtest', impl,
                           '{ROOT_DIR}/tests/games/game' + str(i) + '_moves'],
                    code = 0,
                    stdout_file = '{ROOT_DIR}/tests/games/game' + str(i) + '_gridtest_ref')


for impl in PY_IMPLS:
    for i in range(1, NTESTS + 1):
        ts.add_test('cython_gridtest', impl + '_game' + str(i),
                    cmd = ['python', '{ROOT_DIR}/tests/cython_check.py', 'gridtest', impl,
                           '{ROOT_DIR}/tests/games/game' + str(i) + '_moves'],
                    cwd = '{ROOT_DIR}',
                    env = {'LD_LIBRARY_PATH': '{ROOT_DIR}'},
                    code = 0,
                    stdout_file = '{ROOT_DIR}/tests/games/game' + str(i) + '_gridtest_ref')

ts.save('{BUILD_DIR}/tests.json')
