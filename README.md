# MR acquisition-invariant network

This repository contains experiments and code accompanying the paper:

"MRI Acquisition-invariant representation learning"

which is currently under review.

## Installation

First clone this repository and change directory:
```shell
git clone https://github.com/wmkouw/mrai-net
cd mrai-net
```

Then call the setup script:
```shell
python setup.py install
```
It takes care of installing all dependencies on your system.

`mrai` can now be imported:
```python
from mrai import mrai_cnn
```

## Experiments

- `experiment-brainweb` contains scripts performing experiments using the brainweb simulated data.
- `experiment-mrbrains` contains scripts performing experiments using the [MRBrainS](http://mrbrains13.isi.uu.nl/) dataset.
- `experiment-numparams` contains a script describing the behavior of the network as a function of the number of parameters in the network.


## Contact
Bugs, comments and questions can be submitted to the issues tracker.
