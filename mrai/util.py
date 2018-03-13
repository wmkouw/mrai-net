import scipy as sp
import numpy as np
import nibabel as nib
import scipy.ndimage as nd
from keras import backend as bK
import tensorflow as tf
import sklearn as sk
import sklearn.model_selection as skms
import sklearn.linear_model as sklm
import sklearn.svm as sksv

import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable


def strip_skull(im, mask):
    """Strip the skull from image based on a mask."""

    if mask.dtype is not np.dtype(bool):
        raise ValueError('Mask should be a logical array.')

    # Number of subjects
    num_subjects = im.shape[0]

    # Loop over subjects
    for s in range(num_subjects):

        # Map backgroud pixels to 0
        im[s][mask[s]] = 0

    return im


def viz_segmentation(S, savefn='', cmap='viridis'):
    """Visualise segmentation of image."""

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


def subject2image(fn, imwidth, imheight, slice_index=0, slice_dim=2,
                  segmentation=False, K=[0, 1, 2, 3], flip=False,
                  norm_pix=False):
    """Load subject images."""

    # Find number of subjects
    num_subjects = len(fn)

    # Preallocate images
    ims = np.empty((num_subjects, w, h))

    # Loop over subjects
    for s in range(nS):

        # Read binary and reshape to image
        im = np.fromfile(fn[s], count=w*h, dtype='uint8')
        im = nd.rotate(im.reshape((imwidth, imheight)), 90)

        if segmentation:
            # Restrict classes of segmentations
            labels = np.unique(im)
            for lab in np.setdiff1d(labels, K):
                im[im == lab] = 0

            ims[s, :, :] = im

        else:
            # Normalize pixels
            if norm_pix:
                im[im < 0] = 0
                im[im > 255] = 255
                im = im / 255.

            ims[s, :, :] = im

    return ims


def indices2patches(I, patch_dim=(3, 3), index=np.empty((0,))):
    """Slice patches from an image at particular indices."""

    # Shape
    H,W = I.shape

    # Patch step
    pSH = (patch_dim[0]-1)/2
    pSW = (patch_dim[1]-1)/2

    # Number of patches to sample
    num_samples = index.shape[0]

    # Find 2D index of linear index
    w = np.empty((num_samples,), dtype='int64')
    h = np.empty((num_samples,), dtype='int64')
    if len(index.shape)==1:
        ix = np.empty((num_samples,2), dtype='int64')
        for n in range(num_samples):
            w[n],h[n] = np.unravel_index(index[n], (H,W))
    elif len(index.shape)==2:
        w = index[:,0]
        h = index[:,1]
    else:
        print('index has wrong shape')

    # Slice out patch(es)
    patches = np.empty((num_samples, patch_dim[0], patch_dim[1]))
    for n in range(num_samples):
        patches[n,:,:] =  I[w[n]-pSW:w[n]+pSW+1, h[n]-pSH:h[n]+pSH+1]

    return patches


def images2patches(I, patch_dim=(3,3), edges=(1,1), stride=1):
    ''' Extract all patches of dim size from imageset '''

    # Shapes
    nS,W,H = I.shape

    # Grid coordinates
    wc = range(edges[0],W-edges[0],stride)
    hc = range(edges[1],H-edges[1],stride)

    # Number of patches per image
    nP = len(wc)*len(hc)

    c = 0
    Ip = np.empty((nS*nP,patch_dim[0],patch_dim[1],1))
    for s in range(nS):

        # Form 2D-index array
        tw,th = np.meshgrid(wc,hc)
        indices = np.concatenate((th.reshape((-1,1)),tw.reshape((-1,1))),axis=1)

        # Extract patches at indices
        Ip[c*nP:(c+1)*nP,:,:,0] = indices2patches(I[s,:,:], patch_dim=patch_dim, index=indices)
        c += 1

    return Ip


def cnn(I,L, patch_dim=(31,31), edges=(15,15), batch_size=32, num_epochs=8, stride=1):
    ''' Run a convolutional neural network on the images '''

    # Shapes
    nS,H,W = I.shape
    nK = len(np.unique(L))

    # Extract large patches for images
    Ip = images2patches(I, patch_dim=patch_dim, edges=edges, stride=stride)
    Lp = images2patches(L, patch_dim=(1,1), edges=edges, stride=stride)

    # Switch labels to categorical array
    Lp = ks.utils.to_categorical(Lp - np.min(Lp),nK)

    # Initialize cnn
    model = ks.models.Sequential()
    model.add(ks.layers.Conv2D(32, kernel_size=(3,3),
                     activation='relu',
                     input_shape=(patch_dim[0],patch_dim[1],1)))
    model.add(ks.layers.Conv2D(64, (3, 3), activation='relu'))
    model.add(ks.layers.MaxPooling2D(pool_size=(2, 2)))
    model.add(ks.layers.Dropout(0.25))
    model.add(ks.layers.Flatten())
    model.add(ks.layers.Dense(128, activation='relu'))
    model.add(ks.layers.Dropout(0.5))
    model.add(ks.layers.Dense(nK, activation='softmax'))

    model.compile(loss=ks.losses.categorical_crossentropy,
                  optimizer=ks.optimizers.Adadelta(),
                  metrics=['accuracy'])

    model.fit(Ip, Lp,
              batch_size=batch_size,
              epochs=num_epochs,
              verbose=1)

    return model


def classifier(X,y, complexity='loglin', opt='rmsprop', c_range=np.logspace(-5,4,10), num_folds=2):
    ''' Train classifier in embedding '''

    # Number of classes
    if len(y.shape)==2:
        nK = y.shape[1]
    else:
        nK = len(np.unique(y))

    if complexity=='loglin':

        # Grid search over regularization parameters C
        if X.shape[0] > nK:
            modelgs = skms.GridSearchCV(estimator=sklm.LogisticRegression(multi_class='ovr',solver='lbfgs',class_weight='balanced'),cv=num_folds,param_grid=dict(C=c_range)).fit(X,y)
            bestC = modelgs.best_estimator_.C
        else:
            bestC = 1e6

        # Set up model with optimal C
        return sklm.LogisticRegression(C=bestC, multi_class='ovr', solver='lbfgs', class_weight='balanced')

    elif complexity=='linsvc':

        # Grid search over regularization parameters C
        if X.shape[0] > nK:
            modelgs = skms.GridSearchCV(estimator=sksv.LinearSVC(loss='hinge', class_weight='balanced'), cv=num_folds, param_grid=dict(C=c_range)).fit(X,y)
            bestC = modelgs.best_estimator_.C
        else:
            bestC = 1

        # Train model with optimal C
        return sksv.LinearSVC(C=bestC, loss='hinge', class_weight='balanced')

    elif complexity=='nonlin':

        # Grid search over regularization parameters C
        if X.shape[0] > nK:
            modelgs = skms.GridSearchCV(estimator=sksv.SVC(class_weight='balanced'), cv=num_folds, param_grid=dict(C=c_range)).fit(X,y)
            bestC = modelgs.best_estimator_.C
        else:
            bestC = 1e6

        # Train model with optimal C
        return sksv.SVC(C=bestC, class_weight='balanced')

    elif complexity=='cnn':

        # Initialize cnn
        model = ks.models.Sequential()
        model.add(ks.layers.Conv2D(8, kernel_size=(5,5), activation='relu', padding='valid', input_shape=(X.shape[1],X.shape[2],1)))
        # model.add(ks.layers.Conv2D(16, (3, 3), activation='relu', padding='valid'))
        model.add(ks.layers.MaxPooling2D(pool_size=(2,2), padding='valid'))
        model.add(ks.layers.Dropout(0.2))
        model.add(ks.layers.Flatten())
        model.add(ks.layers.Dense(16, activation='relu'))
        model.add(ks.layers.Dropout(0.2))
        model.add(ks.layers.Dense(8, activation='relu'))
        model.add(ks.layers.Dense(nK, activation='softmax'))

        model.compile(loss=ks.losses.categorical_crossentropy, optimizer=opt, metrics=['accuracy'])

        return model

    else:
        print('Complexity unknown')


def classify(X,y, val=(), complexity='loglin', opt='rmsprop', c_range=np.logspace(-5,4,10), num_folds=2, num_epochs=2, batch_size=32):
    ''' Classify samples in embedding '''

    # Number of classes
    nK = len(np.unique(y))

    if complexity=='loglin':

        # Train regularized classifier
        model = classifier(X,y, complexity='loglin', c_range=c_range, num_folds=num_folds)

        if val:

            # Return error on validation data
            return 1-model.fit(X,y).score(val[0],val[1])

        else:

            # Return error cross-validated over training data
            return np.mean(1-skms.cross_val_score(model, X, y=y, cv=num_folds))

    elif complexity=='linsvc':

        # Train regularized classifier
        model = classifier(X,y, complexity='linsvc', c_range=c_range, num_folds=num_folds)

        if val:
            # Fit and return error on validation data
            return 1 - model.fit(X,y).score(val[0],val[1])

        else:
            # Return error cross-validated over training data
            return np.mean(1 - skms.cross_val_score(model, X, y=y, cv=num_folds))

    elif complexity=='nonlin':

        # Set up regularized classifier
        model = classifier(X,y, complexity='nonlin', c_range=c_range, num_folds=num_folds)

        if val:
            # Fit and return error on validation data
            return 1 - model.fit(X,y).score(val[0],val[1])

        else:
            # Return error cross-validated over training data
            return np.mean(1 - skms.cross_val_score(model, X, y=y, cv=num_folds))

    elif complexity=='cnn':

        # Switch labels to categorical array
        y = ks.utils.to_categorical(y - np.min(y),nK)

        # Set up convolutional network
        model = classifier(X,y, complexity='cnn', opt=opt)

        if val:
            # Validation
            valy = ks.utils.to_categorical(val[1] - np.min(val[1]),nK)

            # Fit model
            model.fit(X,y, batch_size=batch_size, epochs=num_epochs, verbose=1)

            # Fit and return error on validation data
            return 1 - model.test_on_batch(val[0],valy)[1]

        else:
            # Return error cross-validated over training data
            print('X-val not ready')
            return np.mean(1 - skms.cross_val_score(model, X, y=y, cv=num_folds))

    else:
        print('Complexity unknown')


def CMA23(L):
    ''' Map the automatic segmentation to K = {BCK,CSF,GM,WM} '''
    ''' Sets Brainstem and Cerebellum to background (16=0, 6,7,8,45,46,47=0)'''

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
