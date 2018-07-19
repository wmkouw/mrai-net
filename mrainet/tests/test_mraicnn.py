import pytest
import numpy as np
import numpy.random as rn
import tensorflow as tf

from mrainet.mraicnn import MRAIConvolutionalNeuralNetwork


def test_init_patch_size():
    """Valid patch size."""
    with pytest.raises(ValueError):
        N = MRAIConvolutionalNeuralNetwork(patch_size=(-1, 1))


def test_init_dense_size():
    """Valid dense size."""
    with pytest.raises(ValueError):
        N = MRAIConvolutionalNeuralNetwork(dense_size=[])


def test_init_num_kernels():
    """Valid number kernels."""
    with pytest.raises(ValueError):
        N = MRAIConvolutionalNeuralNetwork(num_kernels=[])


def test_contrastive_loss():
    """Test non-negative contrastive loss."""
    N = MRAIConvolutionalNeuralNetwork()
    with tf.Session().as_default():
        assert N.contrastive_loss(label=1, distance=rn.randn(1)).eval() >= 0
        assert N.contrastive_loss(label=0, distance=rn.randn(1)).eval() >= 0


def test_l1_norm():
    """Test non-negative norm."""
    N = MRAIConvolutionalNeuralNetwork()
    with tf.Session().as_default():
        norms = N.l1_norm([rn.randn(100, 1), rn.randn(100, 1)]).eval()
        assert np.all(np.array(norms) >= 0)


def test_l2_norm():
    """Test non-negative norm."""
    N = MRAIConvolutionalNeuralNetwork()
    with tf.Session().as_default():
        norms = N.l2_norm([rn.randn(100, 1), rn.randn(100, 1)]).eval()
        assert np.all(np.array(norms) >= 0)


def test_gen_index_combs():
    """Check whether correct number of combinations are generated."""
    N = MRAIConvolutionalNeuralNetwork()
    x = np.arange(10)
    y = np.arange(10)
    combs = N.gen_index_combs([x, y])
    assert combs.shape[0] == 100
    assert combs.shape[1] == 2


def test_matrix2sparse():
    """Test all pixels are mapped to sparse."""
    A = np.arange(24).reshape((6, 4))
    N = MRAIConvolutionalNeuralNetwork()
    sA = N.matrix2sparse(A)
    assert sA.shape[0] == 24
    assert sA.shape[1] == 3
    assert len(np.setdiff1d(sA[:, 0], np.arange(6))) == 0
    assert len(np.setdiff1d(sA[:, 1], np.arange(4))) == 0
    assert len(np.setdiff1d(sA[:, 2], np.arange(24))) == 0


def test_subsample_rows():
    """Correct shape and contents."""
    A = np.arange(24).reshape((12, 2))
    N = MRAIConvolutionalNeuralNetwork()
    a = N.subsample_rows(A, num_draw=5)
    assert a.shape[0] == 5
    assert a.shape[1] == 2
    assert len(np.unique(a) == 5)


def test_sample_pairs():
    """Produces correct output shape."""
    # Network
    N = MRAIConvolutionalNeuralNetwork(patch_size=(9, 9),
                                       num_kernels=[1],
                                       kernel_size=[(2, 2)],
                                       dense_size=[2],
                                       num_draw=2)

    # Source array
    X = rn.randn(32, 32)
    Y = np.round(np.linspace(0, 3, 32**2)).reshape((32, 32))
    y = N.matrix2sparse(Y, edge=(4, 4))

    # Target array
    Z = rn.randn(32, 32)
    U = np.round(np.linspace(0, 3, 32**2)).reshape((32, 32))
    u = N.matrix2sparse(U, edge=(4, 4))

    # Sample pairs
    P, S = N.sample_pairs(X, y, Z, u, num_draw=(2, 1))

    # Extract pairs
    A, B, a, b = P

    # Check for correct shapes
    assert len(A.shape) == 4
    assert A.shape[1] == A.shape[2]
    assert A.shape[3] == 1
    assert A.shape[0] == a.shape[0]

    assert len(B.shape) == 4
    assert B.shape[1] == B.shape[2]
    assert B.shape[3] == 1
    assert B.shape[0] == b.shape[0]

    # Check for correct contents
    assert len(np.setdiff1d(np.unique(a), [0, 1])) == 0
    assert len(np.setdiff1d(np.unique(b), [0, 1])) == 0
    assert len(np.setdiff1d(np.unique(S), [0, 1])) == 0
