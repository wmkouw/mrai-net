[![Coverage](https://scrutinizer-ci.com/g/wmkouw/mrai-net/badges/coverage.png?b=master)](https://scrutinizer-ci.com/g/wmkouw/mrai-net/statistics/)
[![BuildStatus](https://travis-ci.org/wmkouw/mrai-net.svg?branch=master)](https://travis-ci.org/wmkouw/mrai-net)
![Python version](https://img.shields.io/badge/python-3.4%2C%203.5%2C%203.6-blue.svg)

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

- [MRBrainS](http://mrbrains13.isi.uu.nl/)
- [Brainweb](http://brainweb.bic.mni.mcgill.ca/)

We included the modified version of SIMRI that includes the additional NMR relaxation times, as described in the paper's appendix. In the folder named `simulator` is an example shell script named `run_simulation.sh` to run a simulation using a few default parameters.

## Experiments

- `exp-mrai-cnn_b1b3` contains an experiment using Brainweb1.5T (b1) as source data and Brainweb3.0T (b3) as target data.
- `exp_mrai-cnn_b1mb` contains an experiment using Brainweb1.5T (b1) as source data and MRBrainS (mb) as target data.
- `exp_mrai-cnn_b3mb` contains an experiment using Brainweb3.0T (b3) as source data and MRBrainS (mb) as target data.

## Contact
Bugs, comments and questions can be submitted to the [issues tracker](https://github.com/wmkouw/mrai-net/issues).
