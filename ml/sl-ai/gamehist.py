import numpy as np
import random

ONEHOT_LEN = 15

class FullHist:

    '''
    format:
    initline: `<rand-pos> <rand-val>\n`
    stateline: `<m11> <m12> <m13> <m14> <m22> ... <m44>\n`
    actline: `<move(0->3)> <rand-pos> <rand-val>\n`

    file format:
    <initline>
    <stateline>
    <initline>
    
    <stateline>
    <actline>
    <stateline>
    <actline>
    <stateline>
    <actline>
    ...

    <stateline> (final state when game over)
    '''

    def __init__(self, path):
        self.path = path
        with open(self.path, 'r') as f:
            self.lines = [str(x).strip() for x in f.readlines()]
        self.nmoves = (len(self.lines) - 4) // 2

    def get_state_line(self, i):
        return self.lines[3 + i*2]

    def get_act_line(self, i):
        return self.lines[4 + i*2]

    def get_act_infos(self, i):
        st = list(map(int, self.get_state_line(i).split()))
        act = tuple(map(int, self.get_act_line(i).split()))
        return (st,) + act

    def get_acts_counts(self):
        res = [0, 0, 0, 0]
        for i in range(self.nmoves):
            inf = self.get_act_infos(i)
            res[inf[1]] += 1
        return res
    

class FullHistSet:

    def __init__(self, paths, moves_limit = 1e10, rand_seed = None):
        if rand_seed != None:
            random.seed(rand_seed)

        self.hists = []
        self.nmoves = 0
        
        for p in paths:
            hist = FullHist(p)
            self.hists.append(hist)
            self.nmoves += hist.nmoves
            if self.nmoves >= moves_limit:
                break

    def get_hist_pos(self, i):
        for h in self.hists:
            if i < h.nmoves:
                return (h, i)
            i -= h.nmoves

    def get_acts_counts(self):
        res = [0, 0, 0, 0]
        for h in self.hists:
            sres = h.get_acts_counts()
            for i in range(4):
                res[i] += sres[i]
        return res

    def get_state_line(self, i):
        pos = self.get_hist_pos(i)
        return pos[0].get_state_line(pos[1])

    def get_act_line(self, i):
        pos = self.get_hist_pos(i)
        return pos[0].get_act_line(pos[1])

    def get_act_infos(self, i):
        pos = self.get_hist_pos(i)
        return pos[0].get_act_infos(pos[1])

    def get_batch(self, idxs):
        X = np.zeros((len(idxs), 16*ONEHOT_LEN))
        y = np.zeros((len(idxs),)).astype(np.int)

        for i in range(len(idxs)):
            infos = self.get_act_infos(idxs[i])
            for j in range(16):
                if infos[0][j] != 0:
                    X[i, ONEHOT_LEN * j + infos[0][j] - 1] = 1
            y[i] = infos[1]
                    
        return X, y
    
