#!/bin/bash
if [ $# -ne 5 ];then
	echo "usage: $0 mrcfile tiltfile number_of_threads thickness accelerate(0 for MPI,1 for CUDA) "
	exit 1
fi 
mrcfile=$1
tltfile=$2
threadnum=$3
thickness=$4
accelerate=$5


if [ $accelerate -ne 1 ] && [ $accelerate -ne 0 ];then
	echo "Parameter accelerate must be 1 or 0. 0 for MPI, 1 for CUDA\n"
	exit 1
fi
echo "mrcfile: $mrcfile, tltfile: $tltfile, number of threads: $threadnum, thickness: $thickness, accelerate:$accelerate"

#preprocess
echo "./ICONPreProcess -i $mrcfile -o auto_norm.st"
./ICONPreProcess -i $mrcfile -o auto_norm.st
if [ $? -ne 0 ];then
	echo "ICONPreProcess fail"
	exit 1
fi

#alignment
echo "./markerauto  -i auto_norm.st -a $tltfile -n auto_new.tlt -o auto_ali.xf -r 0 -d -1"
./markerauto  -i auto_norm.st -a $tltfile -n auto_new.tlt -o auto_ali.xf -r 0 -d -1

if [ $? -ne 0 ];then
	echo "alignment fail"
	exit 1
fi

echo "newstack  -input auto_norm.st -xf auto_ali.xf -output auto_fin.mrc"
newstack  -input auto_norm.st -xf auto_ali.xf -output auto_fin.mrc
if [ $? -ne 0 ];then
	echo "alignment fail"
	exit 1
fi

#reconstruction

time mpiexec -n $threadnum ./tomorec3d -i auto_fin.mrc -t auto_new.tlt -g 0,0,0,$thickness -o auto_SIRT.mrc -m SIRT,10,0.2 -G $accelerate 

echo "time mpiexec -n $threadnum ./tomorec3d -i auto_fin.mrc -t auto_new.tlt -g 0,0,0,$thickness -o auto_SIRT.mrc -m SIRT,10,0.2 -G $accelerate "

if [ $? -ne 0 ];then
	echo "reconstruction fail"
	exit 1
fi

echo "complete, output file  is auto_SIRT.mrc"
