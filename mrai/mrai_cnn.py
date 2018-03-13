import numpy as np
import numpy.random as rnd
import nibabel as nib
import tensorflow as tf
from keras import backend as bK
from keras.models import Model, Sequential
from keras.layers import Input, Conv2D, MaxPooling2D, Flatten, Dense, Lambda, \
    Dropout, concatenate, Reshape
from keras.regularizers import l2
import sklearn as sk
import sklearn.model_selection as skms
import sklearn.linear_model as sklm


class mrai_cnn:
    """
    Class of convolutional neural networks for learning an mri-scanner
    acquisition-invariant representation.
    """
    def __init__(self, imsize=(256, 256), classes=np.array([1, 2, 3]),
                 patch_size=(31, 31), num_draw=10, num_targets=1,
                 num_kernels=[8, 8], kernel_size=[3, 3], dense_size=[16],
                 strides=1, dropout=0.5, l2=0.001, margin=1,
                 optimizer='rmsprop', batch_size=32, num_epochs=2):
        """
        Initialize the network with data shape, architecture and
        optimization parameters.
        """
        # Perform assertions on input
        assert len(dense_size) >= 1
        assert len(num_kernels) >= 1
        assert len(num_kernels) >= 1
        assert len(num_kernels) == len(kernel_size)

        # Data shape parameters
        self.imsize = imsize
        self.patch_size = patch_size
        self.hstep = int((patch_size[0]-1)/2)
        self.vstep = int((patch_size[1]-1)/2)
        self.num_draw = num_draw
        self.num_targets = num_targets

        # Classes
        self.classes = classes

        # Network hyperparameters
        self.num_kernels = num_kernels
        self.kernel_size = kernel_size
        self.dense_size = dense_size
        self.strides = strides
        self.dropout = dropout
        self.margin = margin
        self.l2 = l2

        # Optimization parameters
        self.optimizer = optimizer
        self.batch_size = batch_size
        self.num_epochs = num_epochs

        # Initialize net architecture
        self.net = []

    def index2patch(self, I, index=np.zeros((0,))):
        """Slice a patch from an image at given index."""

        # Check whether index is actually an array
        assert len(index.shape) > 0

        # Number of patches to sample
        num_samples = index.shape[0]

        if len(index.shape) == 1:
            # Find 2D index of linear index
            w = np.zeros((num_samples,), dtype='int64')
            h = np.zeros((num_samples,), dtype='int64')

            # For each linear index, find its array index
            for n in range(num_samples):
                w[n], h[n] = np.unravel_index(index[n], self.imsize)

        elif len(index.shape) == 2:
            # First colum is row index, second column is is
            h = index[:, 0]
            w = index[:, 1]
        else:
            raise ValueError('index has wrong shape')

        # Preallocate patch array
        patches = np.zeros((num_samples,
                            self.patch_size[0],
                            self.patch_size[1], 1))

        # For each index, slice out subpatch
        for n in range(num_samples):
            patches[n, :, :, 0] = I[h[n]-self.vstep:h[n]+self.vstep+1,
                                    w[n]-self.hstep:w[n]+self.hstep+1]

        return patches

    def extract_random_patches(self, I, L):
        """Extract a random set of patches from image."""

        # Shapes
        num_images = I.shape[0]
        num_classes = len(self.classes)

        # Preallocate input and output patches
        Ip = np.zeros((num_images*num_classes*self.num_draw,
                       self.patch_size[0], self.patch_size[1], 1))
        Il = np.zeros((num_images*num_classes*self.num_draw,),
                      dtype='int64')

        c = 0
        for i in range(num_images):
            for k in range(num_classes):

                # Cut patch edges from image
                Li = L[i][self.vstep:-self.vstep-1, self.hstep:-self.hstep-1]

                # Find tissue in image
                Lik = (np.argwhere(Li == self.classes[k]) +
                       (self.vstep, self.hstep))

                # Subsample indices
                ixk = rnd.choice(np.arange(Lik.shape[0]),
                                 size=self.num_draw, replace=False)

                # Current patch index
                pix = range(c*self.num_draw, (c+1)*self.num_draw)

                # Sample patch from class k
                Ip[pix, :, :, :] = self.index2patch(I[i], index=Lik[ixk, :])

                # Store class labels
                Il[pix] = self.classes[k]*np.ones((self.num_draw,))

                # Tick up
                c += 1

        return Ip, Il

    def extract_all_patches(self, I, scan_ID=0, edge=(0, 0)):
        """Extract all patches from image, and append a scanner ID var."""

        # Pad image before patch extraction
        Ip = np.pad(I, pad_width=((self.vstep, self.vstep),
                                  (self.hstep, self.hstep)))

        # Sample patches at grid
        Ip = extract_patches_2d(Ip, patch_size=self.patch_size)

        # Augment patches with scanner ID and store
        return np.append(Ip, scan_ID*np.ones((num_patches, 1)), axis=1)

    def contrastive_loss(self, y_true, y_pred):
        """Contrastive loss; squared for similar, """
        return bK.sum(y_true * bK.square(y_pred) +
                      (1-y_true) * bK.abs(bK.maximum(self.margin - y_pred, 0)))

    def l1_norm(self, x):
        """l1-norm for loss layer."""
        return bK.sum(bK.abs(x[0]-x[1]), axis=1, keepdims=True)

    def l2_norm(self, x):
        """l2-norm for loss layer."""
        return bK.sqrt(bK.maximum(bK.sum(bK.square(x[0]-x[1]), axis=1,
                                         keepdims=True), bK.epsilon()))

    def index_comb(self, x):
        """Generate combinations of two index arrays."""
        return np.array(np.meshgrid(x[0], x[1])).T.reshape(-1, 2)

    def compile_net(self):
        ''' Define network architecture '''

        # Setup input layers
        A = Input(shape=(self.patch_size[0], self.patch_size[1], 1))
        B = Input(shape=(self.patch_size[0], self.patch_size[1], 1))
        IDA = Input(shape=(1,))
        IDB = Input(shape=(1,))

        # Convolutional pipeline
        pipeC = Sequential()
        pipeC.add(Conv2D(self.num_kernels[0], (self.kernel_size[0],
                  self.ksernel_size[1]), activation='relu', padding='valid',
                  kernel_regularizer=l2(self.l2), strides=self.strides,
                  input_shape=(self.patch_size[0], self.patch_size[1], 1)))
        pipeC.add(MaxPooling2D(pool_size=(2, 2), padding='valid'))
        pipeC.add(Dropout(self.dropout))
        for n in range(1, len(self.num_kernels)):
            pipeC.add(Conv2D(self.num_kernels[n], (self.kernel_size[n],
                      self.kernel_size[n]), activation='relu', padding='valid',
                      kernel_regularizer=l2(self.l2), strides=self.strides))
            pipeC.add(MaxPooling2D(pool_size=(2, 2), padding='valid'))
            pipeC.add(Dropout(self.dropout))
        pipeC.add(Flatten())

        # Dense pipeline
        pipeD = Sequential()
        pipeD.add(Dense(self.dense_size[0], kernel_regularizer=l2(self.l2),
                  activation='relu', input_shape=(pipeC.output_shape[1] + 1,)))
        for n in range(1, len(self.dense_size)):
            pipeD.add(Dropout(self.dropout))
            pipeD.add(Dense(self.dense_size[n], activation='relu',
                      kernel_regularizer=l2(self.l2)))
        pipeD.add(Dense(2))

        # Distance in embedding space
        distance = Lambda(self.l1_norm, output_shape=(1,))(
                          [pipeD(concatenate([pipeC(A), IDA])),
                           pipeD(concatenate([pipeC(B), IDB]))])

        # Set model
        model = Model(inputs=[A, B, IDA, IDB], outputs=distance)

        # Compile model with optimizer
        model.compile(loss=self.contrastive_loss, optimizer=self.optimizer)

        # Print model
        print(model.summary())

        # Return compiled model
        self.net = model

    def train(self, I, L, J, tix):
        """
        Draw sets of similar and dissimilar pairs of patches from the images
        and use these to train the network in batches.
        """

        # Number of subjects
        num_source = I.shape[0]
        num_target = J.shape[0]

        # Number of classes
        nK = len(self.classes)

        # Minimum of the number of target labels and sample subset
        minTS = min(self.num_targets, self.num_draw)

        # Loop over source subjects
        for i in range(num_source):
            print('At source subject '+str(i+1)+'/'+str(num_source))

            # Loop over target subjects
            for j in range(num_target):
                print('At target subject '+str(j+1)+'/'+str(num_target))

                # Number of pairs in batch
                npb = 2*nK**2*self.num_draw*minTS

                # Pre-allocated data batches
                bI = np.zeros((npb, self.patch_size[0], self.patch_size[1], 1))
                bJ = np.zeros((npb, self.patch_size[0], self.patch_size[1], 1))
                bA = np.zeros((npb, 1))
                bB = np.zeros((npb, 1))
                bY = np.zeros((npb, 1))

                # Check for number of images
                if num_source == 1:
                    o = i
                else:
                    # Indices of other source images
                    o = np.setdiff1d(np.arange(num_source), i)

                    # Draw 1 random image from set
                    o = rnd.choice(o, size=1, replace=False)[0]

                # Cut out current label images
                Li = L[i][self.vstep:-self.vstep-1, self.hstep:-self.hstep-1]
                Lo = L[o][self.vstep:-self.vstep-1, self.hstep:-self.hstep-1]

                # Loop over classes
                c = 0
                for k, classk in enumerate(self.classes):

                    # Find tissue in image
                    Lik = np.argwhere(Li == classk) + (self.vstep, self.hstep)
                    Lok = np.argwhere(Lo == classk) + (self.vstep, self.hstep)

                    # Map multi-index to linear index for class k
                    Lik = np.ravel_multi_index(Lik.T, self.imsize)
                    Lok = np.ravel_multi_index(Lok.T, self.imsize)

                    # Subsample indices of tissues
                    ixk = rnd.choice(Lik, size=self.num_draw, replace=False)
                    ixo = rnd.choice(Lok, size=minTS, replace=False)
                    ixt = rnd.choice(tix[j, k, :].reshape((-1,)), size=minTS,
                                     replace=False)

                    # Generate pairwise index combinations
                    ixkk = self.index_comb((ixk, ixt))
                    ixko = self.index_comb((ixk, ixo))

                    # Patches index
                    pix = range(c*self.num_draw*minTS,
                                (c+1)*self.num_draw*minTS)

                    # Store patches source-target kk
                    bI[pix, :, :, :] = self.index2patch(I[i], index=ixkk[:, 0])
                    bJ[pix, :, :, :] = self.index2patch(J[j], index=ixkk[:, 1])
                    bA[pix, :] = np.zeros((ixkk.shape[0], 1))
                    bB[pix, :] = np.ones((ixkk.shape[0], 1))
                    bY[pix, :] = np.ones((self.num_draw*minTS, 1))
                    c += 1

                    # Patches index
                    pix = range(c*self.num_draw*minTS,
                                (c+1)*self.num_draw*minTS)

                    # Store patches source-source kk
                    bI[pix, :, :, :] = self.index2patch(I[i], index=ixkk[:, 0])
                    bJ[pix, :, :, :] = self.index2patch(I[o], index=ixko[:, 1])
                    bA[pix, :] = np.zeros((ixkk.shape[0], 1))
                    bB[pix, :] = np.zeros((ixko.shape[0], 1))
                    bY[pix, :] = np.ones((self.num_draw*minTS, 1))
                    c += 1

                    # Set of other classes
                    classnotk = np.setdiff1d(self.classes, classk)

                    for l, classl in enumerate(classnotk):

                        # Find tissue in image
                        Lol = np.argwhere(Lo == classl) + (self.vstep,
                                                           self.hstep)

                        # Multi-index to linear index
                        Lol = np.ravel_multi_index(Lol.T, self.imsize)

                        # Subsample indices of other tissues
                        ixo = rnd.choice(Lol, size=minTS, replace=False)
                        ixt = rnd.choice(tix[j, l, :].reshape((-1,)),
                                         size=minTS, replace=False)

                        # Generate pairwise index combinations
                        ixkl = self.index_comb((ixk, ixt))
                        ixol = self.index_comb((ixk, ixo))

                        # Get current patch index
                        pix = range(c*self.num_draw*minTS,
                                    (c+1)*self.num_draw*minTS)

                        # Store sampled patches
                        bI[pix, :, :, :] = self.index2patch(I[i],
                                                            index=ixkl[:, 0])
                        bJ[pix, :, :, :] = self.index2patch(J[j],
                                                            index=ixkl[:, 1])
                        bA[pix, :] = np.zeros((ixkl.shape[0], 1))
                        bB[pix, :] = np.ones((ixkl.shape[0], 1))
                        bY[pix, :] = np.zeros((self.num_draw*minTS, 1))
                        c += 1

                        # Get current patch index
                        pix = range(c*self.num_draw*minTS,
                                    (c+1)*self.num_draw*minTS)

                        # Store sampled patches
                        bI[pix, :, :, :] = self.index2patch(I[i],
                                                            index=ixkl[:, 0])
                        bJ[pix, :, :, :] = self.index2patch(I[o],
                                                            index=ixol[:, 1])
                        bA[pix, :] = np.zeros((ixkl.shape[0], 1))
                        bB[pix, :] = np.zeros((ixol.shape[0], 1))
                        bY[pix, :] = np.zeros((self.num_draw*minTS, 1))
                        c += 1

                # Train on batch and report loss
                self.net.fit(x=[bI, bJ, bA, bB], y=bY,
                             epochs=self.nE, batch_size=self.bS, shuffle=True)

    def propagate(self, Ip, scan_ID):
        """Propagate a set of patches through mrai-network"""
        # Number of patches
        num_patches = Ip.shape[0]

        # Define intermediate-output model
        interm_model = Model(inputs=self.net.input,
                             outputs=self.net.layer[-1].get_output_at(1))

        # Feed forward
        return inter_model.predict([Ip, Ip,
                                    scan_ID*np.ones((num_patches, 1)),
                                    scan_ID*np.ones((num_patches, 1))])

    def segment_image(self, I, model, feed=True, mapost=False, scan_ID=1):
        """Segment a new image using a provided classifier."""

        # Shape of image
        imsize = I.shape

        # Take out patches
        Ip = self.extract_all_patches(I, patch_size=self.patch_size)

        # Feed through network
        if feed:
            Ip = self.propagate(Ip, scan_ID=scan_ID)

        # Classify embedded patches
        preds = model.predict(Ip)

        # If model outputs posteriors, then take maximum a posteriori
        if mapost:
            preds = np.argmax(preds, axis=1) + np.min(self.classes)

        # Reassemble prediction image
        return np.reshape(preds, imsize)
