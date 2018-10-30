import random

def train_online(clf, dataset, classes, train_idxs, test_idxs,
                 epochs, batch_size):
    
    for epoch in range(epochs):
        random.shuffle(train_idxs)

        nbatches = len(train_idxs) // batch_size
        for i in range(nbatches):
            batch_idxs = train_idxs[i*batch_size:(i+1)*batch_size]
            X_batch, y_batch = dataset.get_batch(batch_idxs)

            clf.partial_fit(X_batch, y_batch, classes=classes)
            acc = clf.score(X_batch, y_batch)

            print('Epoch {}[{}/{}]: accuray={:2.4f}%'.format(epoch+1, i+1, nbatches, acc*100))
