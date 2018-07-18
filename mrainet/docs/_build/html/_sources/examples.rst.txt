********
Examples
********

In the /demos folder, there are a number of example scripts. These show potential use cases.

Here we walk through a simple version. First, make sure to import some necessary modules:

.. code-block:: python

    import numpy as np
    import matplotlib.pyplot as plt

    from mrainet.mraicnn import MRAIConvolutionalNeuralNetwork
    from mrainet.util import extract_all_patches
    from mrainet.viz import viz_embedding

Next, we should load some data. The folder `mrainet/demos/data/` contains a source MRI-scan and its segmentation as well as a target MRI-scan with an imcomplete segmentation.

.. code-block:: python

    # Load source MRI-scan and corresponding segmentation
    X = np.load('./demos/data/subject01_GE2D_1.5T.npy')
    Y = np.load('./demos/data/subject01_segmentation.npy')

    # Load target MRI-scan and corresponding segmentation
    Z = np.load('./demos/data/subject02_GE2D_3.0T.npy')
    U = np.load('./demos/data/subject02_segmentation.npy')

    # Note that U is missing a lot of labels
    print('Proportion missing labels = ' + str(np.mean(~np.isnan(U.ravel()))))

Now, it's time to initialize and compile the network.

.. code-block:: python

    # Initialize and compile a small neural network
    N = MRAIConvolutionalNeuralNetwork(patch_size=(31, 31),
                                       num_kernels=[8],
                                       kernel_size=[(3, 3)],
                                       dense_size=[16, 8],
                                       batch_size=128,
                                       num_epochs=4,
                                       num_draw=10,
                                       margin=10)

Note that these options will result in a training set of 220 000 samples, and training might be quite expensive on a CPU laptop.

Now we'll call the training procedure, which automatically handles the pair sampling procedure.

.. code-block:: python

    # Call training procedure on source and target data
    N.train(X, Y, Z, U, num_targets=1)

After training, we'll map all source and target patches extracted from the images to MRAI's learned representation.

.. code-block:: python

    # Extract all source patches and feed them through network.
    PX = extract_all_patches(X[0], patch_size=(31, 31), edge=(15, 15), add_4d=True)
    HX = N.feedforward(PX, scan_ID=0)

    # Map label image to sparse array format
    sY = N.matrix2sparse(Y[0], edge=(15, 15))

    # Extract all target patches and feed them through network.
    PZ = extract_all_patches(Z[0], patch_size=(31, 31), edge=(15, 15), add_4d=True)
    HZ = N.feedforward(PZ, scan_ID=1)

    # Map label image to sparse array format
    sU = N.matrix2sparse(U[0], edge=(15, 15), remove_nans=False)

    # Filter out missing target labels
    HZ = HZ[~np.isnan(sU[:, 2]), :]
    sU = sU[~np.isnan(sU[:, 2]), :]

Given 2-dimensional feature vectors for each patch, we can visualize them using a scatter plot:

.. code-block:: python

    # Create figure
    fig, ax = plt.subplots(figsize=(15, 10))

    # Call visualizer
    viz_embedding(HX, sY[:, 2], marker='o', ax=ax)
    viz_embedding(HZ, sU[:, 2], marker='x', ax=ax)


    