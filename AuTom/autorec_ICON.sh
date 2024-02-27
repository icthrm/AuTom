#!/bin/bash
if [ $# -ne 4 ];then
	echo "usage: $0 mrcfile tiltfile number_of_threads thickness"
	exit 1
fi 
mrcfile=$1
tltfile=$2
threadnum=$3
thickness=$4

echo "mrcfile: $mrcfile, tltfile: $tltfile, number of threads: $threadnum, thickness: $thickness "

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
echo "time mpiexec -n $threadnum ./ICON -input auto_fin.mrc -tiltfile auto_new.tlt -outputPath ./ -slice 0,1023 -ICONIteration 1,7,2 -thr 0.03  -dataType  1"
time mpiexec -n $threadnum ./ICON -input auto_fin.mrc -tiltfile auto_new.tlt -outputPath ./ -slice 0,1023 -ICONIteration 1,7,2 -thr 0.03  -dataType  1
if [ $? -ne 0 ];then
	echo "reconstruction fail"
	exit 1
fi
echo "./ICONMask -inputPath ./reconstruction -tiltfile auto_new.tlt -output ./auto_ICON.mrc -slice 0,1023 -thickness $thickness -crossVfrc ./crossValidation/crossV.frc -fullRecfrc ./crossValidation/fullRec.frc -z 0
"
./ICONMask -inputPath ./reconstruction -tiltfile auto_new.tlt -output ./auto_ICON.mrc -slice 0,1023 -thickness $thickness -crossVfrc ./crossValidation/crossV.frc -fullRecfrc ./crossValidation/fullRec.frc -z 0
if [ $? -ne 0 ];then
	echo "reconstruction fail"
	exit 1
fi
echo "complete, output file  is auto_ICON.mrc"
