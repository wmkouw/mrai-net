# Experiments

Here we describe the experimental protocols from the paper, including choices of parameters.

## Setup

In the paper, we performed three sets of experiments using increasing numbers of labeled target samples.

1. Experiment on simulated data with two different acquisition protocols (Brainweb1.5T -> Brainweb3.0T).
2. Experiment on 1.5T simulated data and 3.0T real data (Brainweb1.5T -> MRBrainS).
3. Experiment on 3.0T simulated data and 3.0T real data (Brainweb3.0T -> MRBrainS).

Each of these experiments is repeated 50 times.

## Results

![alt text](https://github.com/wmkouw/mrai-net/experiments/viz/learning-curve_mraicnn_b1b3.png "Learning curve for Brainweb1.5T -> Brainweb3.0T")

![alt text](https://github.com/wmkouw/mrai-net/experiments/viz/learning-curve_mraicnn_b1mb.png "Learning curve for Brainweb1.5T -> MRBrainS")

![alt text](https://github.com/wmkouw/mrai-net/experiments/viz/learning-curve_mraicnn_b3mb.png "Learning curve for Brainweb3.0T -> MRBrainS")