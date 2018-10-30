import numpy as np
import random

class ModelAgent:

    def __init__(self, model):
        self.model = model

    def get_action(self, grid):
        state = grid.to_arr()
        acts = grid.actions_list()
        X = np.zeros((1, 15 * 16))
        
        for i in range(16):
            if state[i] != 0:
                X[0, 15 * i + state[i] - 1] = 1

        a = self.model.predict(X)[0]
        if a not in acts:
            a = random.choice(acts)
            
        return a
        
