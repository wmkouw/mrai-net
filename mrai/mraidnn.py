#!/usr/bin/env python3
import numpy as np
import numpy.random as rn
from scipy import sparse as sp

from keras import backend as kb
import keras.models as km
import keras.layers as kl
import keras.regularizers as kr
import sklearn.feature_extraction.image as sf


class MRAIDenseNeuralNetwork(object):
    """
    Network for MRI-scanner acquisition-invariant representation learning.

    Class of fully-connected neural networks that aim to map patches of two
    datasets from different MRI-scanners Methods include image processing
    operations, pair sampling and Siamese loss minimization.
    """

    def __init__(self, patch_size=(31, 31), classes=[1, 2, 3], num_draw=10, 
                 dense_size=[16, 8], dropout=0.1, l2=0.001, margin=1,
                 optimizer='rmsprop', batch_size=32, num_epochs=2):
        """
        Initialize with shape, architecture and optimization parameters.

        Parameters
        ----------
        patch_size: tuple(int, int)
            Dimensions of patches extracted from images
        classes: list[int]
            Numerical values of tissue classes
        num_draw : int
            Number of patches to draw from each pair of images
        dense_size : list[int]
            Width of the fully-connected layers
        dropout : float
            Parameter controlling chance of randomly setting weights to 0.
            Dropout encourages convolution kernels to be independent of each
            other.
        l2 : float
            Parameter controlling the amount of regularization applied to the
            gradient update during training.
        margin : 1
            Parameter controlling how far apart dissimilar pairs should be.
        optimizer : str
            Type of optimizer to use for backpropagation.
        num_epochs : int
            Number of times the whole dataset is visited during stochastic
            gradient descent.

        Returns
        -------
        None

        """
        # Checks on data shapes
        if (patch_size[0] < 1) or (patch_size[1] < 1):
            raise ValueError('Negative patch size values.')
        if num_draw < 1:
            raise ValueError('Negative number of samples to draw.')

        # Store data shape parameters
        self.patch_size = patch_size
        self.num_draw = num_draw

        # Classes
        self.classes = classes

        # Check valid architecture sizes
        if len(dense_size) < 1:
                raise ValueError('Too few layers specified.')

        # Network hyperparameters
        self.dense_size = dense_size
        self.dropout = dropout
        self.margin = margin
        self.l2 = l2

        # Optimization parameters
        self.optimizer = optimizer
        self.batch_size = batch_size
        self.num_epochs = num_epochs

        # Initialize net architecture
        self.net = self.compile_net()

    def compile_net(self):
        """Compile network architecture."""
        # Setup input layers
        A = kl.Input(shape=(*self.patch_size, 1))
        B = kl.Input(shape=(*self.patch_size, 1))
        sIDA = kl.Input(shape=(1,))
        sIDB = kl.Input(shape=(1,))

        # Set up sequential pipeline
        prep_net = km.Sequential()
        prep_net.add(kl.Flatten(input_shape=(*self.patch_size, 1)))

        # Full sequential net
        main_net = km.Sequential()
        main_net.add(kl.Dense(self.dense_size[0],
                              activation='relu',
                              kernel_regularizer=kr.l2(self.l2),
                              input_shape=(np.prod(self.patch_size) + 1,)))
        for n in range(1, len(self.dense_size)):
            main_net.add(kl.Dropout(self.dropout))
            main_net.add(kl.Dense(self.dense_size[n], activation='relu',
                                  kernel_regularizer=kr.l2(self.l2)))
        main_net.add(kl.Dense(2))

        # Distance in embedding space
        distance = kl.Lambda(self.l1_norm, output_shape=(1,))(
                          [main_net(kl.concatenate([prep_net(A), sIDA])),
                           main_net(kl.concatenate([prep_net(B), sIDB]))])

        # Set model
        model = km.Model(inputs=[A, B, sIDA, sIDB], outputs=distance)

        # Compile model with optimizer
        model.compile(loss=self.contrastive_loss, optimizer=self.optimizer)

        # Print model
        print(model.summary())

        # Return compiled model
        return model

    def contrastive_loss(self, label, distance):
        """
        Contrastive Siamese loss.

        For similar pairs, it consists of the squared Lp-distance.
        For dissimilar pairs, it consists of a hinge loss with respect to a
        margin parameter.

        Parameters
        ----------
        label : int
            Similarity label, 1=similar and 0=dissimilar
        distance : float
            Lp-norm between pairs of patches mapped through the network.

        Returns
        -------
        float
            Loss value for current pair of patches.

        """
        return kb.sum(label * kb.square(distance) +
                      (1-label) * kb.maximum(self.margin - distance, 0))

    def l1_norm(self, x):
        """l1-norm for loss layer."""
        return kb.sum(kb.abs(x[0] - x[1]), axis=1, keepdims=True)

    def l2_norm(self, x):
        """l2-norm for loss layer."""
        return kb.sum(kb.square(x[0] - x[1]), axis=1, keepdims=True)

    def gen_index_combs(self, x):
        """Generate combinations of two index arrays."""
        return np.array(np.meshgrid(x[0], x[1])).T.reshape(-1, 2)

    def matrix2sparse(self, X):
        """
        Map matrix to a sparse array format.

        Parameters
        ----------
        X : array
            Matrix that should be mapped to sparse array format,
            may contain NaN's.

        Returns
        -------
        sX : array
            Original matrix mapped to (i,j,v) format where i corresponds to the
            i-th row of X, j to the j-column of X and v of the value at
            position (i,j) of X.

        """
        # Shape
        h, w = X.shape

        # Generate meshgrid of coordinates
        tx, ty = np.meshgrid(np.arange(h), np.arange(w))

        # Reshape and stack index and value arrays
        sX = np.vstack((tx.T.ravel(), ty.T.ravel(), X.ravel())).T

        # Remove rows containing NaN's
        sX = sX[~np.isnan(sX[:, 2]), :]

        return sX

    def index2patch(self, X, index):
        """
        Slice patches from an image at given indices.

        Parameters
        ----------
        X : array
            input image
        index : array
            Row and column indices for the provided image.

        Returns
        -------
        patches : array
            Number of patches by patch height by patch width by 1.

        """
        # Check whether index is actually an array
        if not len(index.shape) > 0:
            raise ValueError('Index is an empty array.')

        # Number of patches to sample
        num_samples = index.shape[0]

        # Compute step length from patch center
        vstep = int((self.patch_size[0] - 1)/2)
        hstep = int((self.patch_size[1] - 1)/2)

        if len(index.shape) == 2:

            # First colum is row index, second column is is
            h = index[:, 0]
            w = index[:, 1]

        else:
            raise ValueError('Index has wrong shape.')

        # Preallocate patch array
        patches = np.zeros((num_samples, *self.patch_size, 1))

        # For each index, slice out subpatch
        for n in range(num_samples):
            patches[n, :, :, 0] = X[h[n] - vstep:h[n] + vstep + 1,
                                    w[n] - hstep:w[n] + hstep + 1]

        return patches

    def extract_random_patches(self, X, Y):
        """
        Extract a random set of patches from image.

        Parameters
        ----------
        X : array
            Input image to sample patches from.
        Y : array
            Label image corresponding to X.

        Returns
        -------
        patches : array
            Patches array, num patches by patch height by patch width by 1.
        labels : array
            Tissue label array corresponding to patches array.

        """
        # Shapes
        imsize = X.shape[1:3]
        num_images = X.shape[0]

        # Number of patches
        num_patches = num_images * len(self.classes) * self.num_draw

        # Compute step length from patch center
        vstep = int((self.patch_size[0] - 1)/2)
        hstep = int((self.patch_size[1] - 1)/2)

        # Preallocate input and output patches
        patches = np.zeros((num_patches, *self.patch_size, 1))
        labels = np.zeros((num_patches,), dtype='int64')

        # Define counter
        cnt = 0

        # Loop over number of images
        for i in range(num_images):

            # Loop over classes
            for k, classk in enumerate(self.classes):

                # Cut patch edges from image
                Yi = Y[i][vstep:-vstep-1, hstep:-hstep-1]

                # Find tissue in image
                Yi_k = np.argwhere(Yi == classk) + (vstep, hstep)

                # Subsample indices
                Yi_k = rn.choice(np.ravel_multi_index(Yi_k.T, imsize),
                                 size=self.num_draw, replace=False)

                # Current patch index
                ix = range(cnt*self.num_draw, (cnt+1)*self.num_draw)

                # Sample patch from class k
                patches[ix, :, :, :] = self.index2patch(X[i], Yi_k)

                # Store class labels
                labels[ix] = classk*np.ones((self.num_draw,))

                # Tick up
                cnt += 1

        return patches, labels

    def extract_all_patches(self, X, scan_ID=0, edge=(0, 0)):
        """
        Extract all patches from image, and append a scanner identification.

        Parameters
        ----------
        X : array
            Image to extract all patches from.
        scan_ID : int
            MRI-scanner identification variable.
        edge: tuple(int, int)
            Pixels past this edge should not be sampled from.

        Returns
        -------
        array
            Patches including scanner ID's, num_patches by patch size + 1.

        """
        # Remove edges from image
        X = X[edge[0]:-edge[0], edge[1]:-edge[1]]

        # Compute step length from patch center
        vstep = int((self.patch_size[0] - 1)/2)
        hstep = int((self.patch_size[1] - 1)/2)

        # Pad image before patch extraction
        X = np.pad(X, pad_width=((vstep, vstep), (hstep, hstep)),
                   mode='constant')

        # Sample patches at grid
        patches = sf.extract_patches_2d(X, patch_size=self.patch_size)

        # Number of patches extracted
        num_patches = patches.shape[0]

        # Augment patches with scanner ID and store
        return np.append(patches, scan_ID*np.ones((num_patches, 1)), axis=1)

    def sample_pairs(self, X, y, Z, u, num_draw=(10, 1)):
        """
        Sample a set of pairs of patches from two images.

        Parameters
        ----------
        X : array
            slice from source MRI-scanner
        y : array
            source tissue index sparse array; where each row i,j,k consists of
            the pixel's row index i, the pixel's column index j and the pixel's
            tissue k.
        Z : array
            slice from target MRI-scanner
        u : array
            target tissue index sparse array; where each row i,j,k consists of
            the pixel's row index i, the pixel's column index j and the pixel's
            tissue k.
        num_draw : tuple(int, int)
            maximum number of patches to draw from (source, target)

        Returns
        -------
        P : list[A, B, a, b]
            contains pairs of patches and scanner identifications
        S : array
            contains similarity labels between pairs

        """
        # Check if max draw does not exceed the number of supplied labels
        if (num_draw[0] > y.shape[0]) or (num_draw[1] > u.shape[0]):
            raise ValueError('Insufficient labels provided.')

        # Take only classes present in current slices
        classes = np.intersect1d(np.unique(y[:, 2]), np.unique(u[:, 2]))
        num_classes = len(classes)

        # Number of combinations of patches per class
        num_combs_k = np.prod(num_draw)

        # Total number of patches to draw
        total_num_draw = 2 * num_combs_k * num_classes**2

        # Preallocate patch arrays
        A = np.zeros((total_num_draw, *self.patch_size, 1))
        B = np.zeros((total_num_draw, *self.patch_size, 1))

        # Preallocate scanner label vectors
        a = np.zeros((total_num_draw, 1))
        b = np.zeros((total_num_draw, 1))

        # Preallocate similarity label array
        S = np.zeros((total_num_draw, 1))

        # Counter
        cnt = 0

        # Loop over classes
        for k, classk in enumerate(classes):

            # Take indices of current tissue
            index_yk = y[y[:, 2] == classk, :1]
            index_uk = u[u[:, 2] == classk, :1]

            # Subsample indices
            index_yk = rn.choice(index_yk, size=num_draw[0], replace=False)
            index_uk = rn.choice(index_yk, size=num_draw[1], replace=False)

            ''' Similar pairs from same source image '''

            # Generate pairwise index combinations
            combs = self.gen_index_combs((index_yk, index_yk))

            # Create current patch index pointer
            ix = slice(cnt*num_combs_k, (cnt+1)*num_combs_k)

            # Extract patches and store in arrays
            A[ix, :, :, :] = self.index2patch(X, combs[:, 0])
            B[ix, :, :, :] = self.index2patch(X, combs[:, 1])

            # Store scanner identifications of patches (X=0, Z=1)
            a[ix, :] = np.zeros((num_combs_k, 1))
            b[ix, :] = np.zeros((num_combs_k, 1))

            # Mark these combinations as similar(=1)
            S[ix, :] = np.ones((num_combs_k, 1))

            # Increment counter
            cnt += 1

            ''' Similar pairs from source image and target image '''

            # Generate pairwise index combinations
            combs = self.gen_index_combs((index_yk, index_uk))

            # Create current patch index pointer
            ix = slice(cnt*num_combs_k, (cnt+1)*num_combs_k)

            # Extract patches and store in arrays
            A[ix, :, :, :] = self.index2patch(X, combs[:, 0])
            B[ix, :, :, :] = self.index2patch(Z, combs[:, 1])

            # Store scanner identifications of patches (X=0, Z=1)
            a[ix, :] = np.zeros((num_combs_k, 1))
            b[ix, :] = np.ones((num_combs_k, 1))

            # Mark these combinations as similar(=1)
            S[ix, :] = np.ones((num_combs_k, 1))

            # Increment counter
            cnt += 1

            ''' Similar pairs from same target image '''

            # Generate pairwise index combinations
            combs = self.gen_index_combs((index_uk, index_uk))

            # Create current patch index pointer
            ix = slice(cnt*num_combs_k, (cnt+1)*num_combs_k)

            # Extract patches and store in arrays
            A[ix, :, :, :] = self.index2patch(Z, combs[:, 0])
            B[ix, :, :, :] = self.index2patch(Z, combs[:, 1])

            # Store scanner identifications of patches (X=0, Z=1)
            a[ix, :] = np.ones((num_combs_k, 1))
            b[ix, :] = np.ones((num_combs_k, 1))

            # Mark these combinations as similar(=1)
            S[ix, :] = np.ones((num_combs_k, 1))

            # Increment counter
            cnt += 1

            # Loop over other classes
            for l, classl in enumerate(np.setdiff1d(classk, classes)):

                # Take indices of current tissue
                index_yl = y[y[:, 2] == classl, :1]
                index_ul = u[u[:, 2] == classl, :1]

                # Subsample indices
                index_yl = rn.choice(index_yl, size=num_draw[0], replace=False)
                index_ul = rn.choice(index_ul, size=num_draw[1], replace=False)

                ''' Dissimilar pairs from same source image '''

                # Generate pairwise index combinations
                combs = self.gen_index_combs((index_yk, index_yl))

                # Create current patch index pointer
                ix = slice(cnt*num_combs_k, (cnt+1)*num_combs_k)

                # Extract patches and store in arrays
                A[ix, :, :, :] = self.index2patch(X, combs[:, 0])
                B[ix, :, :, :] = self.index2patch(X, combs[:, 1])

                # Store scanner identifications of patches (X=0, Z=1)
                a[ix, :] = np.zeros((num_combs_k, 1))
                b[ix, :] = np.zeros((num_combs_k, 1))

                # Mark these combinations as dissimilar(=0)
                S[ix, :] = np.zeros((num_combs_k, 1))

                # Increment counter
                cnt += 1

                ''' Dissimilar pairs from source image and target image '''

                # Generate pairwise index combinations
                combs = self.gen_index_combs((index_yk, index_ul))

                # Create current patch index pointer
                ix = slice(cnt*num_combs_k, (cnt+1)*num_combs_k)

                # Extract patches and store in arrays
                A[ix, :, :, :] = self.index2patch(X, combs[:, 0])
                B[ix, :, :, :] = self.index2patch(Z, combs[:, 1])

                # Store scanner identifications of patches (X=0, Z=1)
                a[ix, :] = np.zeros((num_combs_k, 1))
                b[ix, :] = np.ones((num_combs_k, 1))

                # Mark these combinations as dissimilar(=0)
                S[ix, :] = np.zeros((num_combs_k, 1))

                # Increment counter
                cnt += 1

                ''' Dissimilar pairs from same target image '''

                # Generate pairwise index combinations
                combs = self.gen_index_combs((index_uk, index_ul))

                # Create current patch index pointer
                ix = slice(cnt*num_combs_k, (cnt+1)*num_combs_k)

                # Extract patches and store in arrays
                A[ix, :, :, :] = self.index2patch(Z, combs[:, 0])
                B[ix, :, :, :] = self.index2patch(Z, combs[:, 1])

                # Store scanner identifications of patches (X=0, Z=1)
                a[ix, :] = np.ones((num_combs_k, 1))
                b[ix, :] = np.ones((num_combs_k, 1))

                # Mark these combinations as dissimilar(=0)
                S[ix, :] = np.zeros((num_combs_k, 1))

                # Increment counter
                cnt += 1

        # Return collected patches
        return [A, B, a, b], S

    def train(self, X, Y, Z, U):
        """
        Train the network using pairs of patches from the images.

        Parameters
        ----------
        X : array
            source scans, slices by height by width
        Y : array
            source labels, slices by height by width
        Z : array
            target scans, slices by height by width
        U : array
            target labels, slices by height by width,
            contains NaN's at unknown labels

        Returns
        -------
        None

        """
        # Number of subjects from each scanner
        num_source = X.shape[0]
        num_target = Z.shape[0]

        # Loop over source images
        for i in range(num_source):
            print('At source subject '+str(i+1)+'/'+str(num_source))

            # Map source label image to sparse array
            Yi = self.matrix2sparse(Y[i])

            # Loop over target images
            for j in range(num_target):

                # Map target label image to sparse array
                Uj = self.matrix2sparse(U[j])

                # Number of target labels of current image
                num_labels = Uj.shape[0]

                # Draw pairs of patches from two images
                P, S = self.sample_pairs(X[i], Yi, Z[j], Uj,
                                         num_draw=(self.num_draw, num_labels))

                # Train on current set
                self.net.fit(x=P, y=S,
                             epochs=self.num_epochs,
                             batch_size=self.batch_size,
                             validation_split=0.1,
                             shuffle=True,
                             verbose=1)

            # Take random other source image
            o = np.random.choice(np.setdiff1d(np.arange(num_source), i),
                                 size=1, replace=False)[0]

            # Map target label image to sparse array
            Yo = self.matrix2sparse(Y[o])

            # Draw pairs of patches from two images
            P, S = self.sample_pairs(X[i], Yi, X[o], Yo,
                                     num_draw=(self.num_draw, self.num_draw))

            # Train on current set
            self.net.fit(x=P, y=S,
                         epochs=self.num_epochs,
                         batch_size=self.batch_size,
                         validation_split=0.1,
                         shuffle=True,
                         verbose=1)

        # Report
        print('Training complete.')

    def save_model(self, model_fn, weights_fn):
        """
        Save model to filename.

        Parameters
        ----------
        model_fn : str
            Filename to save model to.
        weights_fn : str
            Filename to save weight matrix to.

        Returns
        -------
        None

        """
        # Check if model exists
        if self.net:

            # Serialize model to JSON
            model_json = self.net.to_json()

            # Write to file
            with open(model_fn, 'w') as json_file:
                json_file.write(model_json)

            # Serialize weights to HDF5
            self.net.save_weights(weights_fn)

            # Report
            print('Saved model to disk')

        else:
            raise ValueError('No model compiled yet.')

    def load_model(self, model_fn, weights_fn):
        """
        Load model from filename.

        Parameters
        ----------
        model_fn : str
            Filename of saved model.
        weights_fn : str
            Filename of saved weight matrix.

        Returns
        -------
        None

        """
        # Read json model file
        with open(model_fn, 'r') as json_file:
            loaded_model_json = json_file.read()

        # Construct model from json file
        self.net = km.model_from_json(loaded_model_json)

        # Load weights into new model
        self.net.load_weights(weights_fn)

        # Report
        print("Loaded model from disk")

    def feedforward(self, patches, scan_ID):
        """
        Feed a set of patches forward through the network.

        Parameters
        ----------
        patches : array
            Contains patches in form of number of patches by patch height
            by patch width by 1.
        scan_ID : int
            Scanner identification variable, indicating from which MRI-scanner
            these patches came from.

        Returns
        -------
        array
            Final layer representation of patches fed forward through the
            network.

        """
        # Number of patches
        num_patches = patches.shape[0]

        # Define intermediate-output model
        interm_model = km.Model(inputs=self.net.input,
                                outputs=self.net.layers[-2].get_output_at(1))

        # Scan ID list
        sID = scan_ID*np.ones((num_patches, 1))

        # Feed forward
        return interm_model.predict([patches, patches, sID, sID])

    def segment_image(self, X, model, feed=True, mapost=False, scan_ID=1):
        """
        Segment a new image using the trained network.

        Parameters
        ----------
        X : array
            new image that needs to be segmented.
        model : sklearn-model
            Trained classifier from scikit-learn,
            needs to have a predict method.
        feed : bool
            whether the extracted patches should be fed through the network,
            a value of False is for experimental purposes (def: True)
        mapost : bool
            whether to map the predictions to a maximum a posteriori form.
            (def: False)
        scan_ID : int
            scanner identification of new image.

        Returns
        -------
        preds : array
            Label image of same size as input image, containing predictions
            made by the provided trained classifier.

        """
        # Shape of image
        imsize = X.shape

        # Take out patches
        patches = self.extract_all_patches(X, scan_ID=scan_ID)

        # Feed through network
        if feed:
            patches = self.feedforward(patches, scan_ID=scan_ID)

        # Classify embedded patches
        preds = model.predict(patches)

        # If model outputs posteriors, then take maximum a posteriori
        if mapost:
            preds = np.argmax(preds, axis=1) + np.min(self.classes)

        # Reassemble prediction image
        return np.reshape(preds, imsize)
