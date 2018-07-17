import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable


def viz_segmentation(X, savefn='', cmap='viridis'):
    """
    Visualise segmentation of image.

    Parameters
    ----------
    X : array
        Segmented image that needs to be visualized.
    savefn : str
        Filename to save figure to, (def = '').
    cmap : str
        Colormap in visualization, (def = 'viridis')

    Returns
    -------
    None

    """
    # Figure options
    fig = plt.figure()
    ax = fig.add_subplot(111)

    # Plot prediction
    image = plt.imshow(X, cmap=cmap)

    # Set axes to unvisible
    ax.get_xaxis().set_visible(False)
    ax.get_yaxis().set_visible(False)

    # Add colormap
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.05)
    plt.colorbar(image, cax=cax)

    # Check whether to save or show
    if savefn:
        # Save figure
        fig.savefig(savefn, bbox_inches='tight')

    else:
        plt.show()


def viz_embedding(net, X, Y, Z, U, savefn='', alpha=.3):
    """
    Visualise embedded patches.

    X : array
        Source patches mapped to MRAI-net's representation.
    Y : array
        Tissue labels corresponding to X.
    Z : array
        Target patches mapped to MRAI-net's representation.
    U : array
        Tissue labels corresponding to Z.
    savefn : str
        Filename to save figure to.
    alpha : float
        Parameter indicating how transparent each marker should be.

    Returns
    -------
    None

    """
    # Figure options
    fig = plt.figure()

    # Colors for tissues
    color = ['r', 'b', 'c']

    # Loop over classes
    for k, classk in enumerate(net.classes):

        # Plot labeled source samples in their embedded representation
        plt.plot(X[Y == classk, 0],
                 X[Y == classk, 1],
                 marker='s',
                 color=color[k],
                 alpha=alpha)

        # Plot labeled target samples in their embedded representation
        plt.plot(Z[U == classk, 0],
                 Z[U == classk, 1],
                 marker='x',
                 color=color[k],
                 alpha=alpha)

    # Check whether to save or show
    if savefn:
        # Save figure
        fig.savefig(savefn, bbox_inches='tight')

    else:
        plt.show()