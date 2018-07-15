BrainWeb: Anatomical Model of Normal Brain (BIC / MC Gill)

The anatomical model used to generate simulated brain MRI data consist of a set of 3-dimensional `fuzzy' tissue membership volumes, one for each tissue class (white matter, grey matter, cerebrospinal fluid, fat, ...). The voxel values in these volumes reflects the proportion of tissue present in that voxel, in the range [0,1]. The volumes are defined at a 1mm isotropic voxel grid in Talairach space, with dimensions 181x217x181 (XxYxZ) and start coordinates -90,-126,-72 (x,y,z).

In addition to the fuzzy tissue membership volumes, a discrete anatomical model is provided which consists of a class label (integer) at each voxel, representing the tissue which contributes the most to that voxel.

Volume format : Raw / unsigned char

Tissue Type; T1(ms); T2 (ms); ro; Ki * 10-6
Air; 0; 0; 0; 0
Connective; 500; 70; 0.77; -9.05
CSF; 2569; 329; 1; -9.05
Fat; 350; 70; 1; -7 to -8
Glial matter; 833; 83; 0.86; -9.05
Gray matter; 833; 83; 0.86; -9.05
Muscle; 900; 47; 1; -9 to -10
Skin; 2569; 329; 1; -9.05
Skull; 0; 0; 0; -8.86
White matter; 500; 70; 0.77; -9.05

D.L Collins et al. "Design and construction of a realistic digital brain phantom", IEEE Medical Imaging, Vol 17, N° 3, pp. 463-468, 1998.

http://www.bic.mni.mcgill.ca/brainweb/anatomic_normal.html
