import numpy as np
import random

class RandomAgent:

    def __init__(self, rand_state = None):
        if rand_state != None:
            random.seed(rand_state)

    def get_action(self, grid):
        acts = grid.actions_list()
        a = random.choice(acts)    
        return a
        
