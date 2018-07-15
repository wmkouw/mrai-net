#!/bin/bash
cd "./bin/"
sn=01
TE=4.5
TR=7.9
B0=1.5
th=90
objname="../phantoms/subject"$sn"_256.raw"
fileout="../sims/subject"$sn"_256_seq"$Sq"_TE"$TE"_TR"$TR"_B0"$B0"_th"$th".raw"
./simri test=0 object=4 objname=$objname fileout=$fileout size=256 tacq=2 TE=$TE TR=$TR teta=$th b0=$B0 seqname="GE2D"
