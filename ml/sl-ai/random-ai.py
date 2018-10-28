import random
import sys
import time

from world import World
import env2048

#random.seed(12)

print('random gamer')

for arg in sys.argv:
    print('[{}]'.format(arg))

game = env2048.Grid()

grender = World(4)


game.add_rand_cell()
game.add_rand_cell()

while not game.finished():
    a = random.choice(game.actions_list())
    game.take_action(a)
    game.add_rand_cell()
    grender.score = game.score
    grender.data = game.to_arr()
    grender.render_gui()
    #print(game.to_arr())
    time.sleep(0.1)

print('Score = {}'.format(game.score))
