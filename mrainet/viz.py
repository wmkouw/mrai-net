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

    # Check whether to save figure
    if savefn:
        fig.savefig(savefn, bbox_inches='tight')

    else:
        plt.show()


def viz_embedding(X, Y, classes=[1, 2, 3], marker='o', alpha=.3, ax=[],
                  savefn=''):
    """
    Visualise embedded patches.

    X : array
        Patches mapped to MRAI-net's representation; num_patches by 2.
    Y : array
        Tissue labels corresponding to X; num_patches by 1.
    classes : list(int)
        Numerical values of tissue labels (def = [1, 2, 3])
    marker : str
        Point marker in scatter plot (def = 'o').
    alpha : float
        Parameter indicating how transparent each marker should be (def = 0.3).
    ax : int
        Axis handle to plot in (def = []).
    savefn : str
        Filename to save figure to (def = '')

    Returns
    -------
    None

    """
    if not ax:
        # Figure options
        fig, ax = plt.subplots()

    # Colors for tissues
    color = ['r', 'b', 'c']

    # Loop over classes
    for k, classk in enumerate(classes):

        # Plot labeled source samples in their embedded representation
        ax.scatter(X[Y == classk, 0],
                   X[Y == classk, 1],
                   c=color[k],
                   marker=marker,
                   alpha=alpha)

    # Check whether to save figure
    if savefn:
        fig.savefig(savefn, bbox_inches='tight')

    else:
        plt.show()
