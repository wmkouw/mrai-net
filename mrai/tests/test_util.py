import numpy as np
import numpy.random as rn

from mrai.util import strip_skull


def test_strip_skull():
    I = rn.randn(1, 3, 3)
    M = np.concatenate((np.full((1, 3, 1), True),
                        np.full((1, 3, 2), False)), axis=2)
    J = strip_skull(I, M)
    assert np.all(J[0, :, 0] == 0)
