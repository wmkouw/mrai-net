[![Coverage](https://scrutinizer-ci.com/g/wmkouw/mrai-net/badges/coverage.png?b=master)](https://scrutinizer-ci.com/g/wmkouw/mrai-net/statistics/)
[![BuildStatus](https://travis-ci.org/wmkouw/mrai-net.svg?branch=master)](https://travis-ci.org/wmkouw/mrai-net)
![Python version](https://img.shields.io/badge/python-3.5%2C%203.6-blue.svg)

# MR Acquisition-Invariant Neural Network

This repository contains experiments and code accompanying the paper:

"MR Acquisition-Invariant Representation Learning"

which is currently under review. The preprint is on [ArXiv](https://arxiv.org/abs/1709.07944).

## Installation

mrai-net can be installed through:
```shell
pip install mrai
```

PyPI takes care of all dependencies. However, to ensure that these don't mess up your current python environment, you should set up a virtual one. If you're familiar with [conda](https://conda.io/docs/), you can do this through:
```
conda env create -f environment.yml
source activate mrai
```

## Usage

Demos are available at the online [documentation]().

A simple example of calling the module is:
```python
from mrai.mraicnn import MRAIConvolutionalNeuralNetwork

M = MRAIConvolutionalNeuralNetwork()
M.train(X,Y,Z,U)

H = M.feedforward(X)
```

For more information on individual classes, methods and functions, visit the [docs]().

## Data

In order to isolate MRI-scanner acquisition-based variation in medical images, we simulated datasets of varying scan protocol parameters using [SIMRI](https://www.ncbi.nlm.nih.gov/pubmed/15705518). High-quality human brain phantoms were used, which can be found here:
- [Brainweb](http://brainweb.bic.mni.mcgill.ca/)


The original code for SIMRI can be downloaded from [here](https://sourceforge.net/projects/simri/). Our repo contains a modified version of SIMRI that includes the additional NMR relaxation times, as described in the paper's appendix. For more information on how to run your own simulations, see the example shell script named `run_simulation.sh` in the folder `data/brainweb/simulator`.

Real data consisted of the MRBrainS data set, known from the brain tissue segmentation challenge at MICCAI 2013.
- [MRBrainS](http://mrbrains13.isi.uu.nl/)

We used only the 5 training scans provided, as the labels for the remaining 15 scans have not been released.

## Experiments

Experimental scripts from the paper are included in this repository:
- `exp-mrai-cnn_b1b3`: simulated data from MRI-scanners with different acquisition protocols.
- `exp_mrai-cnn_b1mb`: simulated data as souce and real data as target, for different field strengths.
- `exp_mrai-cnn_b3mb`: simulated data as souce and real data as target, for equivalent field strengths.

For more information on experiment parameters and setup, see the [README](https://github.com/wmkouw/mrai-net/tree/master/experiments) in the experiments folder.

## Contact
Bugs, comments and questions can be submitted to the [issues tracker](https://github.com/wmkouw/mrai-net/issues).
