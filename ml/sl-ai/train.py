import numpy as np
import random
from copy import deepcopy
from sklearn.base import clone

import preprocessing


def train_online(clf, dataset, classes, train_idxs,
                 epochs, batch_size):
    
    for epoch in range(epochs):
        random.shuffle(train_idxs)

        print('Epoch {}/{}...'.format(epoch + 1, epochs))

        nbatches = len(train_idxs) // batch_size
        for i in range(nbatches):
            batch_idxs = train_idxs[i*batch_size:(i+1)*batch_size]
            X_batch, y_batch = dataset.get_batch(batch_idxs)

            clf.partial_fit(X_batch, y_batch, classes=classes)
            acc = clf.score(X_batch, y_batch)

def train_online_early_stop(clf, dataset, classes, train_idxs, test_idxs,
                            epochs, batch_size, improve_limit):

    X_test, y_test = dataset.get_batch(test_idxs)
    best_acc = 0
    best_clf = None
    last_improve = 0
    
    for epoch in range(epochs):
        random.shuffle(train_idxs)

        nbatches = len(train_idxs) // batch_size
        for i in range(nbatches):
            batch_idxs = train_idxs[i*batch_size:(i+1)*batch_size]
            X_batch, y_batch = dataset.get_batch(batch_idxs)

            clf.partial_fit(X_batch, y_batch, classes=classes)

        acc = clf.score(X_test, y_test)
        print('Epoch {}/{}: accuracy = {}'.format(epoch + 1, epochs, acc))
        
        if acc > best_acc:
            last_improve = epoch
            best_acc = acc
            best_clf = deepcopy(clf)

        if epoch - improve_limit > last_improve: break

    return best_clf
                


def train_online_cv(clf, dataset, classes, idxs,
                    epochs, batch_size, cv):

    scores = []    
    sets = preprocessing.kfold_idxs_split(idxs, cv)

    i = 0
    for train_idxs, test_idxs in sets:
        i += 1
        print('CV: pass {}/{}'.format(i, len(sets)))

        clf2 = clone(clf)
        train_online(clf2, dataset, classes, train_idxs, epochs, batch_size)
        X_test, y_test = dataset.get_batch(test_idxs)
        score = clf2.score(X_test, y_test)

        scores.append(score)
    
    return np.array(scores)

def online_cv_predict(clf, idxs, cv,
                      dataset, classes, epochs, batch_size):

    y = np.zeros((len(idxs),))
    i = 0
    sets = preprocessing.kfold_idxs_split(idxs, cv)

    i = 0
    for train_idxs, test_idxs in sets:
        clf2 = clone(clf)
        train_online(clf2, dataset, classes, train_idxs, epochs, batch_size)
        X_test, y_test = dataset.get_batch(test_idxs)
        y_preds = clf2.predict(X_test)
        group_len = len(test_idxs)
        
        y[i*group_len:(i+1)*group_len] = y_preds
        i += 1

    return y
        

        
