import scipy as sp
import numpy as np
import nibabel as nib
import scipy.ndimage as nd
from keras import backend as bK
from keras.models import Model, Sequential
from keras.layers import Input, Conv2D, MaxPooling2D, Flatten, Dense, Lambda, \
    Dropout, concatenate, Reshape
from keras.regularizers import l2
from keras.utils import to_categorical
from keras.losses import categorical_crossentropy
import tensorflow as tf
import sklearn as sk
from sklearn.model_selection import GridSearchCV, cross_val_score
from sklearn.linear_model import LogisticRegression
from sklearn.svm import LinearSVC, SVC
from sklearn.feature_extraction.image import extract_patches_2d

import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable


def strip_skull(I, M):
    """
    Strip the skull from image I based on mask M.

    INPUT   (1) array 'I': number of subjects by height by width
            (2) array 'M': number of subjects by height by width (boolean)
    OUTPUT  (1) array 'I': number of subjects by height by width
    """
    # Check whether mask is a boolean array
    if M.dtype is not np.dtype(bool):
        raise ValueError('Mask should be a logical array.')

    # Loop over subjects
    for subject in range(I.shape[0]):

        # Map backgroud pixels to 0
        I[subject][M[subject]] = 0

    return I


def viz_segmentation(S, savefn='', cmap='viridis'):
    """
    Visualise segmentation of image.

    INPUT   (1) array 'S': number of subjects by height by width
            (2) str 'savefn': filename for saving figure (def:'')
            (3) str 'cmap': colormap (def: 'viridis')
    OUTPUT  None
    """
    # Figure options
    fig = plt.figure()
    ax = fig.add_subplot(111)

    # Plot prediction
    im = plt.imshow(S, cmap=cmap)

    # Set axes properties
    ax.get_xaxis().set_visible(False)
    ax.get_yaxis().set_visible(False)
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.05)
    plt.colorbar(im, cax=cax)

    if savefn:
        fig.savefig(savefn, bbox_inches='tight')
    else:
        plt.show()


def viz_embedding(net, Ih, Lp, Jh, Up, savefn='', alpha=.3):
    """Visualise embedded patches."""
    # Figure options
    fig = plt.figure()
    color = ['r', 'b', 'c']

    # Loop over classes
    for k in range(len(net.K)):
        plt.plot(Ih[Lp == net.K[k], 0], Ih[Lp == net.K[k], 1], 's',
                 color=color[k], alpha=alpha)
        plt.plot(Jh[Up == net.K[k], 0], Jh[Up == net.K[k], 1], 'x',
                 color=color[k], alpha=alpha)

    if savefn:
        fig.savefig(savefn, bbox_inches='tight')
    else:
        plt.show()


def subject2image(fn, imsize=(256, 256), slice_index=0, slice_dim=2,
                  segmentation=False, classes=[0, 1, 2, 3], flip=False,
                  normalization=False):
    """Load subject images."""
    # Find number of subjects
    num_subjects = len(fn)

    # Preallocate images
    ims = np.empty((num_subjects, imsize[0], imsize[1]))

    # Loop over subjects
    for s in range(num_subjects):

        # Read binary and reshape to image
        im = np.fromfile(fn[s], count=np.prod(imsize), dtype='uint8')
        im = nd.rotate(im.reshape(imsize), 90)

        if segmentation:
            # Restrict classes of segmentations
            for lab in np.setdiff1d(np.unique(im), classes):
                im[im == lab] = 0

        else:
            # Normalize pixels
            if normalization:
                im[im < 0] = 0
                im[im > 255] = 255
                im = im / 255.

        # Add image to image array
        ims[s, :, :] = im

    return ims


def extract_all_patches(I, patch_size=(3, 3), edge=(0, 0)):
    """Extract all patches from image."""
    # Remove edges
    I = I[edges[0]:-edges[0], edges[1]:-edges[1]]

    # Step length from center in patch
    vstep = (patch_size[0] - 1) / 2
    hstep = (patch_size[1] - 1) / 2

    # Pad image before patch extraction
    Ip = np.pad(I, pad_width=((vstep, vstep), (hstep, hstep)))

    # Sample patches at grid
    return extract_patches_2d(Ip, patch_size=patch_size)


def set_classifier(X, y, classifier='logreg', c_range=np.logspace(-5, 4, 10),
                   num_folds=2):
    """Construct classifier with optimal regularization parameter."""
    # Data shape
    N = X.shape[0]

    # Number of classes
    if len(y.shape) == 2:
        num_classes = y.shape[1]
    else:
        labels = np.unique(y)
        num_classes = len(labels)

    if classifier == 'logreg':
        # Grid search over regularization parameters C
        if N > num_classes:
            modelgs = GridSearchCV(LogisticRegression(class_weight='balanced'),
                                   cv=num_folds, param_grid=dict(C=c_range))
            modelgs.fit(X, y)
            bestC = modelgs.best_estimator_.C
        else:
            bestC = 1

        # Set up model with optimal C
        return LogisticRegression(C=bestC, class_weight='balanced')

    elif classifier == 'linsvc':
        # Grid search over regularization parameters C
        if N > num_classes:
            modelgs = GridSearchCV(LinearSVC(class_weight='balanced'),
                                   cv=num_folds, param_grid=dict(C=c_range))
            modelgs.fit(X, y)
            bestC = modelgs.best_estimator_.C
        else:
            bestC = 1

        # Train model with optimal C
        return LinearSVC(C=bestC, class_weight='balanced')

    elif classifier == 'rbfsvc':
        # Grid search over regularization parameters C
        if N > num_classes:
            modelgs = GridSearchCV(SVC(class_weight='balanced'),
                                   cv=num_folds, param_grid=dict(C=c_range))
            modelgs.fit(X, y)
            bestC = modelgs.best_estimator_.C
        else:
            bestC = 1e6

        # Train model with optimal C
        return SVC(C=bestC, class_weight='balanced')

    elif classifier == 'convnn':
        # Sequentially add network layers
        model = Sequential()
        model.add(Conv2D(8, kernel_size=(3, 3),
                         activation='relu',
                         padding='valid',
                         kernel_regularizer=l2(0.001),
                         input_shape=(X.shape[1], X.shape[2], 1)))
        model.add(MaxPooling2D(pool_size=(2, 2), padding='valid'))
        model.add(Dropout(0.2))
        model.add(Flatten())
        model.add(Dense(16, activation='relu', kernel_regularizer=l2(0.001)))
        model.add(Dropout(0.2))
        model.add(Dense(8, activation='relu', kernel_regularizer=l2(0.001)))
        model.add(Dense(num_classes, activation='softmax'))

        model.compile(loss=categorical_crossentropy, optimizer='rmsprop',
                      metrics=['accuracy'])

        return model
    else:
        print('Classifier unknown')


def classify(X, y, val=[], classifier='logreg', c_range=np.logspace(-5, 4, 10),
             num_folds=2, num_epochs=2, batch_size=32, verbose=True):
    """Classify sets of patches."""
    # Number of classes
    num_classes = len(np.unique(y))

    if classifier == 'convnn':
        # Switch labels to categorical array
        y = to_categorical(y - np.min(y), num_classes)
    else:
        # Check for feature vectors
        if len(X.shape) > 2:
            X = X.reshape((X.shape[0], -1))

    # Train regularized classifier
    model = set_classifier(X, y, classifier=classifier, c_range=c_range,
                           num_folds=num_folds)

    if classifier.lower() in ['logreg', 'linsvc', 'rbfsvc']:
        if val:
            # Number of validation samples
            N = val[0].shape[0]

            # Error on validation data
            err = 1 - model.fit(X, y).score(val[0].reshape((N, -1)), val[1])

        else:
            # Error cross-validated over training data
            err = np.mean(1 - cross_val_score(model, X, y=y, cv=num_folds))

    elif classifier == 'convnn':
        if val:
            # Validation
            valy = to_categorical(val[1] - np.min(val[1]), num_classes)

            # Fit model
            model.fit(X, y,
                      batch_size=batch_size,
                      epochs=num_epochs,
                      validation_split=0.2,
                      shuffle=True)

            # Error on validation data
            err = 1 - model.test_on_batch(val[0], valy)[1]

        else:
            # Error cross-validated over training data
            err = np.mean(1 - cross_val_score(model, X, y=y, cv=num_folds))
    else:
        raise ValueError('Classifier unknown')

    # Report
    if verbose:
        print('Classification error = ' + str(err))

    return err


def CMA_to_4classes(L):
    """
    Map CMA's automatic segmentation to {BCK,CSF,GM,WM}.

    CMA's automatic segmentation protocol lists over 80 different tissue 
    classes. Here we map these back to the four we used in the paper: 
    background (BCK), cerebrospinal fluid (CSF), gray matter (GM) and 
    white matter (WM). Sets Brainstem and Cerebellum to background 
    (16=0, 6,7,8,45,46,47=0).
    """
    # Number of subjects
    nI = L.shape[0]

    # Re-map to
    L = -L
    for i in range(nI):
        L[i][L[i] == -0] = 0
        L[i][L[i] == -1] = 0
        L[i][L[i] == -2] = 3
        L[i][L[i] == -3] = 2
        L[i][L[i] == -4] = 1
        L[i][L[i] == -5] = 1
        L[i][L[i] == -6] = 0
        L[i][L[i] == -7] = 0
        L[i][L[i] == -8] = 0
        L[i][L[i] == -9] = 2
        L[i][L[i] == -10] = 2
        L[i][L[i] == -11] = 2
        L[i][L[i] == -12] = 2
        L[i][L[i] == -13] = 2
        L[i][L[i] == -14] = 1
        L[i][L[i] == -15] = 1
        L[i][L[i] == -16] = 0
        L[i][L[i] == -17] = 2
        L[i][L[i] == -18] = 2
        L[i][L[i] == -19] = 2
        L[i][L[i] == -20] = 2
        L[i][L[i] == -21] = 0
        L[i][L[i] == -22] = 0
        L[i][L[i] == -23] = 2
        L[i][L[i] == -24] = 1
        L[i][L[i] == -25] = 0
        L[i][L[i] == -26] = 2
        L[i][L[i] == -27] = 2
        L[i][L[i] == -28] = 2
        L[i][L[i] == -29] = 0
        L[i][L[i] == -30] = 0
        L[i][L[i] == -31] = 0
        L[i][L[i] == -32] = 0
        L[i][L[i] == -33] = 0
        L[i][L[i] == -34] = 0
        L[i][L[i] == -35] = 0
        L[i][L[i] == -36] = 0
        L[i][L[i] == -37] = 0
        L[i][L[i] == -38] = 0
        L[i][L[i] == -39] = 0
        L[i][L[i] == -40] = 0
        L[i][L[i] == -41] = 3
        L[i][L[i] == -42] = 2
        L[i][L[i] == -43] = 1
        L[i][L[i] == -44] = 1
        L[i][L[i] == -45] = 0
        L[i][L[i] == -46] = 0
        L[i][L[i] == -47] = 0
        L[i][L[i] == -48] = 2
        L[i][L[i] == -49] = 2
        L[i][L[i] == -50] = 2
        L[i][L[i] == -51] = 2
        L[i][L[i] == -52] = 2
        L[i][L[i] == -53] = 2
        L[i][L[i] == -54] = 2
        L[i][L[i] == -55] = 2
        L[i][L[i] == -56] = 2
        L[i][L[i] == -57] = 0
        L[i][L[i] == -58] = 2
        L[i][L[i] == -59] = 2
        L[i][L[i] == -60] = 2
        L[i][L[i] == -61] = 0
        L[i][L[i] == -62] = 0
        L[i][L[i] == -63] = 0
        L[i][L[i] == -64] = 0
        L[i][L[i] == -65] = 0
        L[i][L[i] == -66] = 0
        L[i][L[i] == -67] = 0
        L[i][L[i] == -68] = 0
        L[i][L[i] == -69] = 0
        L[i][L[i] == -70] = 0
        L[i][L[i] == -71] = 0
        L[i][L[i] == -72] = 0
        L[i][L[i] == -73] = 0
        L[i][L[i] == -74] = 0
        L[i][L[i] == -75] = 0
        L[i][L[i] == -76] = 0
        L[i][L[i] == -77] = 0
        L[i][L[i] == -78] = 0
        L[i][L[i] == -79] = 0
        L[i][L[i] == -80] = 0
        L[i][L[i] == -81] = 0
        L[i][L[i] == -82] = 0
        L[i][L[i] == -83] = 0
        L[i][L[i] == -84] = 3

    return L
