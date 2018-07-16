# Experiments

Here we describe the experimental protocols from the paper, including choices of parameters.

## Setup

In the paper, we performed three sets of experiments using increasing numbers of labeled target samples.

1. Experiment on simulated data with two different acquisition protocols (Brainweb1.5T -> Brainweb3.0T).
2. Experiment on 1.5T simulated data and 3.0T real data (Brainweb1.5T -> MRBrainS).
3. Experiment on 3.0T simulated data and 3.0T real data (Brainweb3.0T -> MRBrainS).

Each of these experiments is repeated 50 times.

## Results

![alt text](https://github.com/wmkouw/mrai-net/blob/master/experiments/viz/learning-curve_mraicnn_b1b3.png "Learning curve for Brainweb1.5T -> Brainweb3.0T")

Even with very few labeled target samples, mrai-net is removing most of the scanner-based variation (blue line is much lower than red line in left subplot) and it immediately outperforms both the source and target CNN's. Note also that using the source scanners (with more than 1000 labeled patches per class) is actually worse than the target scanner patches (with <10 labeled patches per class); the scanners are so different, that including source data actually disrupts training a target classifier. <br>

![alt text](https://github.com/wmkouw/mrai-net/blob/master/experiments/viz/learning-curve_mraicnn_b1mb.png "Learning curve for Brainweb1.5T -> MRBrainS")

In this experiment, we show that the same pattern for purely simulated data also holds when using simulated data and applying it to real data. <br>

![alt text](https://github.com/wmkouw/mrai-net/blob/master/experiments/viz/learning-curve_mraicnn_b3mb.png "Learning curve for Brainweb3.0T -> MRBrainS")

For this last experiment, the scanners are much more similar (red line in left subplot is much lower). As such, the source classifier is much more appropriate for the target scanner and it outperforms the target CNN (purple below black line in right subplot).