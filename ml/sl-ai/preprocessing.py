import random

def train_test_idxs_split(total_size, data_size, test_prop):
    idxs = list(range(total_size))
    random.shuffle(idxs)

    test_size = int(data_size * test_prop)
    train_size = data_size - test_size
    train_idxs = idxs[:train_size]
    test_idxs = idxs[train_size:train_size + test_size]
    return train_idxs, test_idxs


def kfold_idxs_split(idxs, k):

    sets = []
    group_size = len(idxs)//k

    for i in range(k):
        test_idxs = idxs[i*group_size:(i+1)*group_size]
        train_idxs = idxs[:i*group_size] + idxs[(i+1)*group_size:]
        sets.append((train_idxs, test_idxs))

    return sets
