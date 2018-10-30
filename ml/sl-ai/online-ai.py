import random
import sys
import time

from world import World
import gamehist
import glob
from modelagent import ModelAgent
from envapp import EnvApp
import preprocessing
import train

from sklearn.linear_model import SGDClassifier
from sklearn.externals import joblib
from sklearn.metrics import accuracy_score

#random.seed(12)

FILES = './ml/data/full-*.out'
CLASSES = [0, 1, 2, 3]
DATA_SIZE = 20000
TEST_PROP = 0.2
EPOCHS = 2
BATCH_SIZE = 64

files = glob.glob(FILES)

ds = gamehist.FullHistSet(files, moves_limit = DATA_SIZE)
train_idxs, test_idxs = preprocessing.train_test_idxs_split(ds.nmoves, DATA_SIZE, TEST_PROP)

clf = SGDClassifier(loss='log')

train.train_online(clf, ds, CLASSES, train_idxs, test_idxs, EPOCHS, BATCH_SIZE)
        
X_test, y_test = ds.get_batch(test_idxs)
y_pred = clf.predict(X_test)
acc = accuracy_score(y_test, y_pred)
print('Test accuracy: {}'.format(acc))


#sets = preprocessing.kfold_idxs_split(train_idxs, k = 10)
#print(len(sets[0][0]), len(sets[0][1]))

#X_train, y_train = ds.get_batch(train_idxs)
#y_pred = clf.predict(X_train)
#acc = accuracy_score(y_train, y_pred)
#print('Train accuracy: {}'.format(acc))
           


#pred = clf.predict(X)
#print(pred)
#print(y)

joblib.dump(clf, 'ai_model.joblib')
