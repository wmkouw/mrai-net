import pytest
import numpy as np
import numpy.random as rn

from mrai.mraicnn import MRAIConvolutionalNeuralNetwork


def test_init():
    """Test initialization values."""
    with pytest.raises(ValueError):
        N = MRAIConvolutionalNeuralNetwork(patch_size=(-1, 1))

    N = MRAIConvolutionalNeuralNetwork()
    assert len(N.net) == 0


def test_matrix2sparse():
    """Assert matrix2sparse is correct shape."""
    # Generate random matrix
    N = MRAIConvolutionalNeuralNetwork()
    A = rn.randn(10, 3)
    sA = N.matrix2sparse(A)
    assert sA.shape[1] == 3
    assert sA.shape[0] == np.prod(A.shape)
