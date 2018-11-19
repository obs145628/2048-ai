import random
import sys
import time

from world import World
import gamehist
import glob
from modelagent import ModelAgent
from envapp import EnvApp
import optimization
import preprocessing
import train

from sklearn.linear_model import SGDClassifier
from sklearn.externals import joblib
from sklearn.metrics import accuracy_score, confusion_matrix, precision_score, recall_score, f1_score

#random.seed(12)

FILES = './ml/data/full-*.out'
CLASSES = [0, 1, 2, 3]
DATA_SIZE = 20000
TEST_PROP = 0.2
EPOCHS = 1000
BATCH_SIZE = 64
GRID_SEARCH_CV = 5
GRID_SEARCH_EPOCHS = 2
EARLY_STOP_LIMIT = 10

files = glob.glob(FILES)

ds = gamehist.FullHistSet(files, moves_limit = DATA_SIZE)
train_idxs, test_idxs = preprocessing.train_test_idxs_split(ds.nmoves, DATA_SIZE, TEST_PROP)

clf = SGDClassifier(loss='log')

params = [
    {'loss': ['hinge', 'log'], 'penalty': ['none', 'l1', 'l2'] }
]

def eval_model(clf):

    y_preds = train.online_cv_predict(clf, train_idxs, 5, ds, CLASSES, GRID_SEARCH_EPOCHS, BATCH_SIZE)
    X_train, y_train = ds.get_batch(train_idxs)

    print('Confusion matrix:')
    print(confusion_matrix(y_train, y_preds))

    print('Accuracy:', accuracy_score(y_train, y_preds))
    #print('Precision:', precision_score(y_train, y_preds))
    #print('Recal:', recall_score(y_train, y_preds))
    #print('F1 Score:', f1_score(y_train, y_preds))
    

'''
grid_search = optimization.OnlineGridSearchCV(clf, params, GRID_SEARCH_CV, ds, CLASSES, GRID_SEARCH_EPOCHS, BATCH_SIZE)
grid_search.fit(train_idxs)
print(grid_search.best_score_)
print(grid_search.best_params_)
clf = grid_search.best_estimator_
'''

eval_model(clf)

clf = train.train_online_early_stop(clf, ds, CLASSES, train_idxs, test_idxs, EPOCHS, BATCH_SIZE, EARLY_STOP_LIMIT)

X_test, y_test = ds.get_batch(test_idxs)
y_pred = clf.predict(X_test)
acc = accuracy_score(y_test, y_pred)
print('Test accuracy: {}'.format(acc))


joblib.dump(clf, 'ai_model.joblib')
