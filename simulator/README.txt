------------------------------------------------------------------------------------------------

                               SIMRI: 3D MRI Simulator

------------------------------------------------------------------------------------------------

# Summary
#---------

  SIMRI is an open source C Software. SIMRI is 3D MRI simulator based on the Bloch equations.
  This simulator  proposes an efficient management of the T2* effect and integrates in a unique simulator
  most of the simulation features that are offered in different simulators. It takes into account the main
  static field value and enables realistic simulations of the chemical shift artifact including off-resonance
  phenomena. It also simulates the artifacts linked to the static field inhomogeneity like those induced by
  susceptibility variation within an object. It is implemented in the C language and the MRI sequence programming
  is done using high level C functions with a simple programming interface. To manage large simulations, the
  magnetization kernel is implemented in a parallelized way that enables simulation on PC grid architecture.
  It is a portable software running on Windows and Linux environnement.

# Related publications
#-----------------------
[1] H. Benoit-Cattin, G. Collewet, B. Belaroussi, H. Saint-Jalmes, and C. Odet, “The SIMRI project:
A versatile and interactive MRI simulator”, Journal of Magnetic Resonance, vol. 173, pp. 97-115, 2005.
[2] H. Benoit-Cattin, F. Bellet, J. Montagnat, C. Odet, “Magnetic Resonance Imaging (MRI) simulation on
a grid computing architecture”, In Proc. of IEEE CGIGRID'03- BIOGRID'03, Tokyo, 2003.

# Authors & Institutions
#-----------------------
  * Hugues BENOIT-CATTIN
        CREATIS, UMR CNRS #5515, U 630 Inserm, Université Claude Bernard Lyon 1, INSA Lyon, Bât. B. Pascal, 69621 Villeurbanne, France
  * Guylaine COLLEWET
        CEMAGREF / Food Processes Engineering Research Unit, 17 av de Cucillé, 35044 Rennes, France.
  * Boubakeur BELAROUSSI
        CREATIS, UMR CNRS #5515, U 630 Inserm, Université Claude Bernard Lyon 1, INSA Lyon, Bât. B. Pascal, 69621 Villeurbanne, France
  * Christophe ODET
        CREATIS, UMR CNRS #5515, U 630 Inserm, Université Claude Bernard Lyon 1, INSA Lyon, Bât. B. Pascal, 69621 Villeurbanne, France
  * Hervé SAINT-JALMES
        Laboratoire de Résonance Magnétique Nucléaire - Méthodologie et Instrumentation en Biophysique, UMR CNRS 5012, Université Claude Bernard Lyon 1 - CPE Lyon, France
  * Fabrice BELLET
        CREATIS, UMR CNRS #5515, U 630 Inserm, Université Claude Bernard Lyon 1, INSA Lyon, Bât. B. Pascal, 69621 Villeurbanne, France
  * Alexis AMADON
        CEA, SHFJ-UNAF, 4 place du General Leclerc, 91400 Orsay, France


# Acknowledgements
#-----------------
The SIMRI project sarted in June 2000 at CREATIS, Lyon France.
The SIMRI project has been initiated thanks to the work done by G. Soufflet on the initial 1D MRI simulator of J. Bittoun.
This work is partly supported by the IST European Data-Grid Project and the French ministry for research ACI-GRID project.
This work has been also funded by the INSA Lyon.
We want to thank S. Balac from the CNRS MAPLY lab for its contribution on the susceptibility artifact simulation.
Many thanks to F. Bellet and J. Montagnat for their help in the SIMRI parallelisation and grid
implementation.

# License
#---------
 The SIMRI software is distributed under the CeCiLL license (http://www.cecill.info/index.en.html).
 The CeCiLL license is a free-software license, created under the supervision of the three biggest
 research institutions on computer sciences in France :
   - CNRS  (http://www.cnrs.fr)
   - CEA   (http://www.cea.fr)
   - INRIA (http://www.inria.fr)
 The CeCiLL license is compatible with the GNU GPL : you can legally redistribute SIMRI based programs in GPL.
 You have to RESPECT this license. Particularly, all distributed programs using parts of the SIMRI sources MUST
 be open-source. Please carefully read the license terms.

# Package structure
#--------------------
The directory simri is organised as follows :
 - src/ contains all the source codes (.c, .h) and linux makefile
     . Main file : simri.c
     . Virtual object files : object.c,object-ki.c,object-compound.c,object-simple.c,object-brain.c
     . Experience, noise and filtering files :experience.c, noise.c, hanning.c
     . Sequence files : seq-tf.c,seq-ge.c,seq-mpi.c,seq-tse.c,seq-ir.c,seq-se.c,seq-fid.c,sequence.c,seq-special.c
     . Event files : event.c,event-precession.c,event-rfpulse.c,event-acquisition.c
     . Magnetisation kernel files: kernel.c,kernel-precession.c,kernel-rfpulse.c,kernel-frequency-acq.c,kernel-phase-acq.c
     . Reconstruction and output files :reconstruction.c,output.c,display.c
     . Test files :test-sequence.c,test-rmn.c, test-se.c

 - src/libido-lite contains part of the source codes (.c, .h) of the C 'libido' library developed by CREATIS that is used
 by simri. All the functions starting by Id... and all the types starting by P... are from the Libido Library. This library
 mainly offers functions to allocate, access, free, write/read any type of signals (PSIGNAL), images (PPIMAGE), volumes (PPPVOLUME)
 and sequences (PPPPSEQUENCE).


 - doc/ contains the script to generate the full documentation usin 'doxygen' tool.(see http://www.doxygen.org)
     .doc\html\index.html points to the main page of the documentation generated by doxygen

 - msvc/ contains the files required for the code compilation under windows
     .under MSVC++ : simri3d.dsw, simri3d.dsp, libidolite.dsp
     .under MS.NET : simri3d.sln,simri3d.vcproj, libidolite.vcproj
     .Linux makefile are in src/

 - data/ contains the date files
     .rf/ : contains Rf pulses
     .brainbic/ : contains a brain object built from the digital brain phantom of McGill Brain imaging center.
     Note that the brainlabelZ256.raw is copied in /bin and in /src in order to ease the test with the brain object in grid environment as well as on PC.

 - bin/ contains affimwin.exe and the dedicated dll (idogdllvc.dll glib-1.3.dll) used under windows for the IdImaDisplay function
 of the libido library. It offers a quick way of viewing object and image without any specific image viewer software. The same functionnality
is obtained under Linux if you set the dispay in 24 bits. Anyway, you can desactivate the display by setting the preprocessor variable BROKEN_DISPLAY.


# Getting started
#-----------------
  1) Unzip simri.zip (simri.tgz)
  2) Build the simri executable code. Note that first the libido-lite library is first built.
  3) Run the code with a simple test like : 'simri3d test=210'
  4) Play with other parameters (like test=0 object=4 seqname=TF2D). Read the main file simri.c
  5) Enter the code to really discover simri.

# Mailing list
#-----------------
Simri has its mailing list : simri@creatis.insa-lyon.fr
If you are interested, subscribe to the simri mailing list.
http://www.creatis.insa-lyon.fr/mailman/listinfo/simri

# Contributions
#-----------------
We wish you will contribute to the development of the simri project like by proposing new sequences,
new objects, new models of pulse, noise, RF ...

# Note for MPI running
#----------------------
Required Linux packages
   . LAM (for example lam-6.5.6-4 for a redhat-7.3)

Modify the Makefile to :
   . compile with mpicc instead of gcc
   . add the flag -DHAVE_MPI
   . add theflag -static at the link step if you plan to install the softin a similar computer that does not have the libido library installed.

Lam configuration :
   . create localy a file named lam-bhosts.def, that containsa list of the computer that defines the cluster,
   one per line with the number of processors per computer like the following :

     localhost
     tux cpu=2
     frodo
     merry

   . define the shell variable LAMRSH=ssh to define the remote access of the lam forker server.
    If ssh is chosen, configure all the public keys of the targetted computers, and activate the
    ssh-agent, in order to avoid the passwd authentification.
   . Run lamboot ./lam-bhosts.def > It spawns a server process on each computer defined in the lam-bhosts.def file.
   . Check the MPI run, using the lamnodes command.
   . End a MPI session by killing all the lam process using the lamhalt command.

Running SIMRI :
   . The executable code must be available on all the nodes, in the SAME directory than the one given at the program launch.
   . To run the program : mpirun -np <number of copies> simri

     - <number of copies> equals the number of process to be run.
     This number is independant from the node number of the cluster, but generally for monoprocessor node,
     we set this number to the node numbers.

     Example : mpirun -np 4 ./simri suppose that the simri program is available in the $HOME directory of each cluster node.

------------------------------------------------------------------------------------------------
# Specific information concerning B1 field management
***
I. B1 map file format :
When using the inhomogeneousB1 option in SIMRI, this B1map directory should include the B1t.txt and B1r.txt files that contain the real and imaginary parts 
of the circularly-polarized B1-field used for transmission and reception respectively . These files should be written in ASCII form, starting with the
normalized complex B1-value of the voxel located in the negative image corner and ending with that of the voxel in the positive corner. Then x is the 
fastest increment index while z is the slowest.
The B1t and B1r files must be provided by the user with dimensions equal to that of the image.

***
II. How to obtain B1t and B1r ?
If z is oriented in the B0 direction and if the x-y-z frame is a direct frame, the proton spins will spin in the clockwise direction when facing the 
z direction, i.e. will spin in the negative orientation when using the trigonometric convention. In that case, 

B1t (transmission field) = B1- = 1/2 (Bx - i By)*
 
while 

B1r (reception field) = B1+ = 1/2 (Bx + i By) 

where B1x and B1y are the complex field values computed from electromagnetic simulation programs such as Microwave Studio or Microstripes. 
If z is oriented opposite the BO direction, then B1t = B1+, and B1r = B1-, as seen most often in the 
literature (cf. Van de Moortele et al. MRM 54:1503-1518 (2005) for instance).

***
III. What changes were induced in the SIMRI computation ?
In addition to the flip angle and phase changes induced by inhomogeneous B1t (cf. changes in kernel-rfpulse.c), 
the simulated signal integrates magnetization weighted by RF coil sensitivity profile B1r*(r) : 

signal = integral[M(r) * B1r*(r)]

This is performed at the end of the "experience.c" file.

***
IV. Example : 
For instance, the simulation of a 20-cm-diameter sphere in a transmit/receive quadrature RF probe at 11.7 T. in Microwave Studio yielded the central axial 
slice results compiled in the H_Cir2P-CW.txt file. These were then used to produce a 128x128 B1t 2D map with 2-mm resolution in b1_cir2p-cw.xls . The final
result with the format described above was compiled in b1t_sphere_128x128_11.7T.txt, which is then pointed by the B1t.txt link file. Because of the sphere 
symmetry and the fact that the receive probe is the same as the transmit probe in this case, it was unnecessary to run another simulation to produce the 
B1r file (which would have been done by swapping the phase on the x and y ports): B1r can indeed directly be obtained from B1t in such a case : 

Re(B1r(x,y)) = Im(B1t(y,x)) and Im(B1r(x,y)) = Re(B1t(y,x))

This is performed in experience.c, in case the B1r.txt file is not provided.
Then the simri code can be executed with the inhomogeneousB1 flag set to 1:

simri object=5 size=128 inhomogeneousB1=1 seqname=GE2D
(if seqname=SE2D, this does not work - have not found why yet)

