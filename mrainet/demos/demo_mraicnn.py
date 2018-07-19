import numpy as np
import matplotlib.pyplot as plt

from mrainet.mraicnn import MRAIConvolutionalNeuralNetwork
from mrainet.util import extract_all_patches
from mrainet.viz import viz_embedding

'''Load data set'''

# Load source MRI-scan and corresponding segmentation
X = np.load('./data/subject01_GE2D_1.5T.npy')
Y = np.load('./data/subject01_segmentation.npy')

# Load target MRI-scan and corresponding segmentation
Z = np.load('./data/subject02_GE2D_3.0T.npy')
U = np.load('./data/subject02_segmentation.npy')

# Note that U is missing a lot of labels
print('Proportion missing labels = ' + str(np.mean(~np.isnan(U.ravel()))))

'''Set up MRAI network'''

# Initialize and compile a small neural network
N = MRAIConvolutionalNeuralNetwork(patch_size=(31, 31),
                                   num_kernels=[8],
                                   kernel_size=[(3, 3)],
                                   dense_size=[16, 8],
                                   batch_size=128,
                                   num_epochs=4,
                                   num_draw=10,
                                   margin=10)

'''Train the net'''

# Call training procedure on source and target data
N.train(X, Y, Z, U, num_targets=1)

'''Map images to MRAI representation.'''

# Extract all source patches and feed them through network.
PX = extract_all_patches(X[0], patch_size=(31, 31), add_4d=True)
HX = N.feedforward(PX, scan_ID=0)

# Map label image to sparse array format
sY = N.matrix2sparse(Y[0])

# Extract all target patches and feed them through network.
PZ = extract_all_patches(Z[0], patch_size=(31, 31), add_4d=True)
HZ = N.feedforward(PZ, scan_ID=1)

# Map label image to sparse array format
sU = N.matrix2sparse(U[0], remove_nans=False)

# Filter out missing target labels
HZ = HZ[~np.isnan(sU[:, 2]), :]
sU = sU[~np.isnan(sU[:, 2]), :]

'''Visualize results'''

# Create figure
fig, ax = plt.subplots(figsize=(15, 10))

# Call visualizer
viz_embedding(HX, sY[:, 2], marker='o', ax=ax)
viz_embedding(HZ, sU[:, 2], marker='x', ax=ax)
