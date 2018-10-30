import os
from sklearn.externals import joblib

from modelagent import ModelAgent
from envapp import EnvApp

path = os.environ.get('MODEL')

clf = joblib.load(path)
agent = ModelAgent(clf)
app = EnvApp(agent, with_gui = True)
app.play_game()
