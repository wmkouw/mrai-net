import numpy as np
import numpy.random as rn

from sklearn.datasets import make_classification

from mrainet.util import strip_skull, extract_all_patches, classify


def test_strip_skull():
    X = rn.randn(1, 3, 3)
    M = np.concatenate((np.full((1, 3, 1), True),
                        np.full((1, 3, 2), False)), axis=2)
    X = strip_skull(X, M)
    assert np.all(X[0, :, 0] == 0)


def test_extract_all_patches():
    X = rn.randn(31, 33)
    P = extract_all_patches(X, patch_size=(3, 3), edge=(0, 0))
    assert P.shape[1] == P.shape[2]


def test_classify_lr():
    X, y = make_classification(n_samples=100, n_features=3)
    Z, u = make_classification(n_samples=50, n_features=3)
    err = classify(X, y, val=[Z, u], classifier='lr')
    assert not np.isnan(err)
    assert (0 <= err <= 1)

# def test_classify_cnn():
#     X, y = make_classification(n_samples=100, n_features=3)
#     Z, u = make_classification(n_samples=50, n_features=3)
#     err = classify(X, y, val=[Z, u], classifier='lr')
#     assert not np.isnan(err)
#     assert (0 <= err <= 1)
