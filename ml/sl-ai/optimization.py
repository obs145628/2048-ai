from sklearn.base import clone

import train

class ModelOptimizer:

    def __init__(self, models):

        self.models = models

    def fit(self, X, y):

        self.best_estimator_ = None
        self.best_name_ = None
        self.best_params_ = None
        self.best_score_ = float('-inf')

        results = []

        for name in self.models:
            model = self.models[name]

            print('Evaluation of `{}`...'.format(name))

            model.fit(X, y)
            estimator = model.best_estimator_
            params = model.best_params_
            score = model.best_score_

            results.append((name, score))

            if score > self.best_score_:
                self.best_estimator_ = estimator
                self.best_name_ = name
                self.best_params_ = params
                self.best_score_ = score

        self.results = results


def build_params_comb_rec(res, current, rest):
    if len(rest) == 0:
        res.append(current)
        return

    for name in rest: break

    vals = rest[name]
    rest2 = dict(rest)
    rest2.pop(name)

    for val in vals:
        current2 = dict(current)
        current2[name] = val
        build_params_comb_rec(res, current2, rest2)

        
def get_params_comb(params):
    res = []
    for p in params:
        build_params_comb_rec(res, {}, p)
    return res
        
class OnlineGridSearchCV:

    def __init__(self, clf, params, cv, dataset, classes, epochs, batch_size):
        self.clf = clf
        self.params = params
        self.cv = cv

        self.dataset = dataset
        self.classes = classes
        self.epochs = epochs
        self.batch_size = batch_size

    def fit(self, idxs, _ = None):

        self.best_estimator_ = None
        self.best_params_ = None
        self.best_score_ = float('-inf')

        params_list = get_params_comb(self.params)

        for i in range(len(params_list)):
            params = params_list[i]

            print('GridSearch: Evaluating model {}/{}...'.format(i+1, len(params_list)))

            clf = clone(self.clf)
            clf.set_params(**params)

            score = train.train_online_cv(clf, self.dataset, self.classes, idxs,
                                          self.epochs, self.batch_size, self.cv).mean()

            if score > self.best_score_:
                self.best_estimator_ = clf
                self.best_params_ = params
                self.best_score_ = score
        

    
