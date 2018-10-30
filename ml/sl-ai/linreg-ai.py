import random
import sys
import time

from world import World
import gamehist
import glob
from modelagent import ModelAgent
from envapp import EnvApp
import preprocessing

from sklearn.linear_model import SGDClassifier
from sklearn.externals import joblib
from sklearn.metrics import accuracy_score

#random.seed(12)

# FILES = './tests/games16k/full*.out'
FILES = './tt/data/full-*.out'
DATA_SIZE = 20000
TEST_PROP = 0.2
EPOCHS = 1000
BATCH_SIZE = 64

files = glob.glob(FILES)

ds = gamehist.FullHistSet(files, moves_limit = DATA_SIZE)
train_idxs, test_idxs = preprocessing.train_test_idxs_split(ds.nmoves, DATA_SIZE, TEST_PROP)

clf = SGDClassifier(loss='log')

for epoch in range(EPOCHS):
    random.shuffle(train_idxs)

    nbatches = len(train_idxs) // BATCH_SIZE
    for i in range(nbatches):
        batch_idxs = train_idxs[i*BATCH_SIZE:(i+1)*BATCH_SIZE]

        X_batch, y_batch = ds.get_batch(batch_idxs)

        clf.partial_fit(X_batch, y_batch, classes=[0,1,2,3])

        print('Epoch {}[{}/{}]: '.format(epoch+1, i+1, nbatches))
        

X_test, y_test = ds.get_batch(test_idxs)
y_pred = clf.predict(X_test)
acc = accuracy_score(y_test, y_pred)
print('Test accuracy: {}'.format(acc))

X_train, y_train = ds.get_batch(train_idxs)
y_pred = clf.predict(X_train)
acc = accuracy_score(y_train, y_pred)
print('Train accuracy: {}'.format(acc))
           


#pred = clf.predict(X)
#print(pred)
#print(y)

joblib.dump(clf, 'ai_model.joblib')
