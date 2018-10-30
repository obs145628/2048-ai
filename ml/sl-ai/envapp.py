import time

from world import World
import env2048

class EnvApp:

    def __init__(self, agent, with_gui = False):
        self.agent = agent
        self.with_gui = with_gui
        
    def play_game(self):

        game = env2048.Grid()
        grender = World(4)

        game.add_rand_cell()
        game.add_rand_cell()

        while not game.finished():
            a = self.agent.get_action(game)
            if a not in game.actions_list():
                raise Exception('Invalid action')
            
            game.take_action(a)
            game.add_rand_cell()

            grender.score = game.score
            grender.data = game.to_arr()
            if self.with_gui:
                grender.render_gui()
            else:
                print(game.to_arr())

            time.sleep(0.1)

        print('Score = {}'.format(game.score))
        return game
