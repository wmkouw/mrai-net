import numpy as np
import numpy.random as rn

from mrai.util import strip_skull


def test_strip_skull():
    X = rn.randn(1, 3, 3)
    M = np.concatenate((np.full((1, 3, 1), True),
                        np.full((1, 3, 2), False)), axis=2)
    X = strip_skull(X, M)
    assert np.all(X[0, :, 0] == 0)
