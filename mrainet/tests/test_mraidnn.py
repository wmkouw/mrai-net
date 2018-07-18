import pytest
import numpy as np
import numpy.random as rn
import tensorflow as tf

from mrainet.mraidnn import MRAIDenseNeuralNetwork


def test_init_patch_size():
    """Valid patch size."""
    with pytest.raises(ValueError):
        N = MRAIDenseNeuralNetwork(patch_size=(-1, 1))


def test_init_dense_size():
    """Valid dense size."""
    with pytest.raises(ValueError):
        N = MRAIDenseNeuralNetwork(dense_size=[])


def test_contrastive_loss():
    """Test non-negative contrastive loss."""
    N = MRAIDenseNeuralNetwork()
    with tf.Session().as_default():
        assert N.contrastive_loss(label=1, distance=rn.randn(1)).eval() >= 0
        assert N.contrastive_loss(label=0, distance=rn.randn(1)).eval() >= 0


def test_l1_norm():
    """Test non-negative norm."""
    N = MRAIDenseNeuralNetwork()
    with tf.Session().as_default():
        norms = N.l1_norm([rn.randn(100, 1), rn.randn(100, 1)]).eval()
        assert np.all(np.array(norms) >= 0)


def test_l2_norm():
    """Test non-negative norm."""
    N = MRAIDenseNeuralNetwork()
    with tf.Session().as_default():
        norms = N.l2_norm([rn.randn(100, 1), rn.randn(100, 1)]).eval()
        assert np.all(np.array(norms) >= 0)


def test_gen_index_combs():
    """Check whether correct number of combinations are generated."""
    N = MRAIDenseNeuralNetwork()
    x = np.arange(10)
    y = np.arange(10)
    combs = N.gen_index_combs([x, y])
    assert combs.shape[0] == 100
    assert combs.shape[1] == 2


def test_matrix2sparse():
    """Test all pixels are mapped to sparse."""
    A = np.arange(24).reshape((6, 4))
    print(A)
    N = MRAIDenseNeuralNetwork()
    sA = N.matrix2sparse(A)
    print(sA)
    assert sA.shape[0] == 24
    assert sA.shape[1] == 3
    assert len(np.setdiff1d(sA[:, 0], np.arange(6))) == 0
    assert len(np.setdiff1d(sA[:, 1], np.arange(4))) == 0
    assert len(np.setdiff1d(sA[:, 2], np.arange(24))) == 0

# def test_subsample_rows():