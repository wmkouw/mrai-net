import numpy as np
import scipy as sp
import scipy.ndimage as nd

import keras.models as km
import keras.layers as kl
import keras.regularizers as kr
import keras.utils as ku

import sklearn.model_selection as sm
import sklearn.linear_model as sl
import sklearn.svm as sv
import sklearn.feature_extraction.image as sf


def strip_skull(X, M):
    """
    Strip the skull from image X based on mask M.

    Parameters
    ----------
    X : array
        MRI-scan that includes the skull of the subject.
    M : array
        Mask array indicating the skull with True.

    Returns
    -------
    X : array
        MRI-scan with 0's at the mask indices.

    """
    # Check whether mask is a boolean array
    if M.dtype is not np.dtype(bool):
        raise ValueError('Mask should be a logical array.')

    # Check if X contains multiple subjects
    if len(X.shape) > 2:

        # Check for same number of subjects in mask and data.
        if not (X.shape[0] == M.shape[0]):
            raise ValueError('Shape mismatch.')
        
        # Strip skull, for each subject
        for i in range(X.shape[0]):
            X[i][M[i]] = 0

    else:
        # Strip skull
        X[M] = 0

    return X


def subject2image(fn, imsize=(256, 256), segmentation=False, 
                  classes=[0, 1, 2, 3], normalization=False):
    """
    Load subject images.

    Parameters
    ----------
    fn : str
        Filename of image.
    imsize : tuple(int, int)
        Dimensions of the image to load.
    segmentation : bool
        Indicating whether this is a label matrix.
    classes : list[int]
        List of numerical values of tissue classes.
    normalization : bool
        Whether to normalize the image.

    Returns
    -------
    X : array
        Loaded images, number of subjects by image height by image width

    """
    # Find number of subjects
    num_subjects = len(fn)

    # Preallocate images
    X = np.empty((num_subjects, imsize[0], imsize[1]))

    # Loop over subjects
    for s, fn_s in enumerate(fn):

        # Read binary
        X_s = np.fromfile(fn_s, count=np.prod(imsize), dtype='uint8')

        # Reshape binary list to image
        X_s = nd.rotate(X_s.reshape(imsize), 90)

        # Check for label image
        if segmentation:

            # Restrict classes of segmentations
            for classk in np.setdiff1d(np.unique(X_s), classes):
                X_s[X_s == classk] = 0

        else:
            # Normalize pixels
            if normalization:
                im[im < 0] = 0
                im[im > 255] = 255
                im = im / 255.

        # Add image to image array
        X[s, :, :] = X_s

    return X


def extract_all_patches(X, patch_size=(3, 3), edge=(0, 0)):
    """
    Extract all patches from image.

    Parameters
    ----------
    X : array
        Image to extract patches from.
    patch_size : tuple(int, int)
        Dimensions of patches to extract.
    edge : tuple(int, int)
        Dimensions of the outer edge to ignore during sampling.

    Returns
    -------
    patches : array
        Patches array with number of samples by patch height by patch width.

    """
    # Remove edges
    X = X[edge[0]:-edge[0], edge[1]:-edge[1]]

    # Step length from center in patch
    vstep = (patch_size[0] - 1) / 2
    hstep = (patch_size[1] - 1) / 2

    # Pad image before patch extraction
    X = np.pad(X, pad_width=((vstep, vstep), (hstep, hstep)), mode='constant')

    # Sample patches at grid
    return sf.extract_patches_2d(X, patch_size=patch_size)


def set_classifier(X, y, classifier='lr', c_range=np.logspace(-5, 4, 10),
                   num_folds=2):
    """
    Construct classifier with optimal regularization parameter.

    Parameters
    ----------
    X : array
        Data matrix, number of samples by number of features.
    y : array
        Label vector or matrix, number of samples by 1, or number of samples by
        number of classes.
    classifier : str
        Type of classifier to use, options are: 'lr' (logistic regression),
        'linsvc' (linear support vector classifier),
        'rbfsvc' (radial basis function support vector classifier),
        'cnn' (convolutional neural network), (def='lr').
    c_range : array
        Range of possible regularization parameters,
        (def=np.logspace(-5, 4, 10)).
    num_folds : int
        Number of folds to use in cross-validation.

    Returns
    -------
    sklearn classifier
        A trained classifier with a predict function.

    """
    # Data shape
    N = X.shape[0]

    # Number of classes
    if len(y.shape) == 2:

        # Check number of classes by number of columns of label matrix
        num_classes = y.shape[1]

    else:
        # Check number of classes by the number of unique entries
        num_classes = len(np.unique(y))

    if classifier == 'lr':

        # Grid search over regularization parameters C
        if N > num_classes:

            # Set up a grid search cross-validation procedure
            modelgs = sm.GridSearchCV(sl.LogisticRegression(),
                                      param_grid=dict(C=c_range),
                                      cv=num_folds)

            # Fit grid search model
            modelgs.fit(X, y)

            # Extract best regularization parameter
            bestC = modelgs.best_estimator_.C

        else:
            # Standard regularization parameter
            bestC = 1

        # Set up model with optimal C
        return sl.LogisticRegression(C=bestC)

    elif classifier == 'linsvc':

        # Grid search over regularization parameters C
        if N > num_classes:

            # Set up a grid search cross-validation procedure
            modelgs = sm.GridSearchCV(sv.LinearSVC(),
                                      param_grid=dict(C=c_range),
                                      cv=num_folds)

            # Fit grid search model
            modelgs.fit(X, y)

            # Extract best regularization parameter
            bestC = modelgs.best_estimator_.C

        else:
            # Standard regularization parameter
            bestC = 1

        # Train model with optimal C
        return sv.LinearSVC(C=bestC)

    elif classifier == 'rbfsvc':

        # Grid search over regularization parameters C
        if N > num_classes:

            # Set up a grid search cross-validation procedure
            modelgs = sm.GridSearchCV(sv.SVC(), cv=num_folds,
                                      param_grid=dict(C=c_range))

            # Fit grid search model
            modelgs.fit(X, y)

            # Extract best regularization parameter
            bestC = modelgs.best_estimator_.C

        else:
            # Standard regularization parameter
            bestC = 1

        # Train model with optimal C
        return sv.SVC(C=bestC)

    elif classifier == 'cnn':

        # Start sequential model
        net = km.Sequential()

        # Convolutional part
        net.add(kl.Conv2D(8, kernel_size=(3, 3),
                          activation='relu',
                          padding='valid',
                          kernel_regularizer=kr.l2(0.001),
                          input_shape=(X.shape[1], X.shape[2], 1)))
        net.add(kl.MaxPooling2D(pool_size=(2, 2), padding='valid'))
        net.add(kl.Dropout(0.2))
        net.add(kl.Flatten())

        # Fully-connected part
        net.add(kl.Dense(16, activation='relu',
                         kernel_regularizer=kr.l2(0.001)))
        net.add(kl.Dropout(0.2))
        net.add(kl.Dense(8, activation='relu',
                         kernel_regularizer=kr.l2(0.001)))
        net.add(kl.Dense(num_classes, activation='softmax'))

        # Compile network architecture
        net.compile(loss='categorical_crossentropy',
                    optimizer='rmsprop',
                    metrics=['accuracy'])

        return net

    else:
        print('Classifier unknown')


def classify(X, y, val=[], classifier='lr', c_range=np.logspace(-5, 4, 10),
             num_folds=2, num_epochs=2, batch_size=32, verbose=True):
    """
    Classify sets of patches.

    Parameters
    ----------
    X : array
        Data matrix, number of samples by number of features.
    y : array
        Label vector or matrix, number of samples by 1, or number of samples by
        number of classes.
    val : list[X, y]
        Validation data and label set, (def = []).
    classifier : str
        Type of classifier to use, options are: 'lr' (logistic regression),
        'linsvc' (linear support vector classifier),
        'rbfsvc' (radial basis function support vector classifier),
        'cnn' (convolutional neural network), (def='lr').
    c_range : array
        Range of possible regularization parameters,
        (def=np.logspace(-5, 4, 10)).
    num_folds : int
        Number of folds to use in cross-validation.
    num_epochs : int
        Number of epochs in training a neural network.
    batch_size : int
        Size of the batches to cut the data set into.
    verbose : bool
        Verbosity during training.

    Returns
    -------
    err : float
        Classification error.

    """
    # Number of classes
    num_classes = len(np.unique(y))

    if classifier == 'cnn':

        # Switch labels to categorical array
        y = ku.to_categorical(y - np.min(y), num_classes)

    else:
        # If data has more than 2 dimensions, reshape it back to 2.
        if len(X.shape) > 2:
            X = X.reshape((X.shape[0], -1))

    # Train regularized classifier
    model = set_classifier(X, y, classifier=classifier, num_folds=num_folds,
                           c_range=c_range)

    if classifier.lower() in ['lr', 'linsvc', 'rbfsvc']:

        if val:

            # Number of validation samples
            N = val[0].shape[0]

            # Error on validation data
            err = 1 - model.fit(X, y).score(val[0].reshape((N, -1)), val[1])

        else:
            # Error cross-validated over training data
            err = np.mean(1 - sm.cross_val_score(model, X, y=y, cv=num_folds))

    elif classifier == 'cnn':

        if val:
            # Validation
            valy = ku.to_categorical(val[1] - np.min(val[1]), num_classes)

            # Fit model
            model.fit(X, y, batch_size=batch_size, epochs=num_epochs,
                      validation_split=0.2, shuffle=True)

            # Error on validation data
            err = 1 - model.test_on_batch(val[0], valy)[1]

        else:
            # Error cross-validated over training data
            err = np.mean(1 - sm.cross_val_score(model, X, y=y, cv=num_folds))
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

    Parameters
    ----------
    L : array
        Label matrix

    Returns
    -------
    L : array
        Label matrix

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
