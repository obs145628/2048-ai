from envapp import EnvApp
from randomagent import RandomAgent

agent  = RandomAgent(rand_state = None)
app = EnvApp(agent, with_gui=True)
app.play_game()
