import numpy as np
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

from sklearn.linear_model import LogisticRegression
from sklearn.externals import joblib
from sklearn.model_selection import cross_val_score, GridSearchCV
from sklearn.metrics import accuracy_score
from sklearn.svm import SVC
from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.decomposition import PCA, KernelPCA
from sklearn.manifold import LocallyLinearEmbedding

from xgboost import XGBClassifier

#random.seed(12)

FILES = './ml/data/full-*.out'
CLASSES = [0, 1, 2, 3]
DATA_SIZE = 2000
TEST_PROP = 0.2
GRID_SEARCH_CV = 5
GRID_SEARCH_SCORING = 'accuracy'
PCA_NC = -1
KPCA_NC = -1
LLE_NC = 10

files = glob.glob(FILES)

ds = gamehist.FullHistSet(files, moves_limit = DATA_SIZE)
train_idxs, test_idxs = preprocessing.train_test_idxs_split(ds.nmoves, DATA_SIZE, TEST_PROP)
X_train, y_train = ds.get_batch(train_idxs)
X_test, y_test = ds.get_batch(test_idxs)

MODELS = {
    'logreg': (LogisticRegression(multi_class='auto'),
               [
                   {'penalty': ['l1', 'l2'], 'solver': ['liblinear', 'saga']}
               ]),

    'svm': (SVC(kernel='rbf'),
            [
                {'kernel': ['linear', 'poly', 'rbf'], 'gamma': ['auto', 'scale'], 'decision_function_shape': ['ovo', 'ovr']}
            ]),

    'knn': (KNeighborsClassifier(),
            [
                {'n_neighbors': [5, 10, 15, 20, 25], 'metric': ['euclidean', 'manhattan', 'chebyshev',
                                                        'jaccard', 'matching']}
            ]),

    'random_forest': (RandomForestClassifier(),
                      [
                          {'n_estimators': [5, 10, 20, 50], 'max_features': [2, 4, 6, 8, 10], 'bootstrap': [True, False]}
                      ])
}



def train_model(name):

    clf = MODELS[name][0]
    params = MODELS[name][1]
    grid_search = GridSearchCV(clf, params, cv = GRID_SEARCH_CV,
                               scoring = GRID_SEARCH_SCORING)
    grid_search.fit(X_train, y_train)
    print(grid_search.best_params_)
    print(grid_search.best_score_)
    return grid_search.best_estimator_


def eval_models(models, X, y):

    models_grids = dict()
    for model in models:
        clf = models[model][0]
        params = models[model][1]
        grid_search = GridSearchCV(clf, params, cv = GRID_SEARCH_CV,
                                   scoring = GRID_SEARCH_SCORING)
        models_grids[model] = grid_search


    opt = optimization.ModelOptimizer(models_grids)
    opt.fit(X, y)

    print(opt.results)
    print('Best model: `{}` with score {}'.format(opt.best_name_, opt.best_score_))
    print(opt.best_params_)
    return opt.best_estimator_

if PCA_NC != -1:
    pca = PCA(n_components = PCA_NC)
    X_train = pca.fit_transform(X_train)
    X_test = pca.transform(X_test)

if KPCA_NC != -1:
    kpca = KernelPCA(n_components=KPCA_NC, kernel='rbf', gamma=0.04)
    X_train = kpca.fit_transform(X_train)
    X_test = kpca.transform(X_test)

if LLE_NC != -1:
    lle = LocallyLinearEmbedding(n_components=LLE_NC, n_neighbors=10)
    X_train = lle.fit_transform(X_train)
    X_test = lle.transform(X_test)
    
#clf = train_model('knn')
clf = eval_models(MODELS, X_train, y_train)

scores = cross_val_score(clf, X_train, y_train, cv=10, scoring='accuracy')
print('CV accuracy: mean = {}, std = {}'.format(scores.mean(), scores.std()))
        
y_pred = clf.predict(X_test)
acc = accuracy_score(y_test, y_pred)
print('Test accuracy: {}'.format(acc))

y_pred = clf.predict(X_train)
acc = accuracy_score(y_train, y_pred)
print('Train accuracy: {}'.format(acc))

joblib.dump(clf, 'ai_model.joblib')

