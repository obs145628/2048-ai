import os
import sys

mpath = sys.argv[1]
fpath = sys.argv[2]

with open(fpath, 'r') as f:
    lines = [str(x).strip() for x in f.readlines()]

vals = list(map(int, lines[-1].split(' ')))
score = vals[-1]
print(vals)
print(score)

if 14 in vals:
    os.rename(mpath, './data/moves-{}.out'.format(score))
    os.rename(fpath, './data/full-{}.out'.format(score))
else:
    os.remove(mpath)
    os.remove(fpath)
