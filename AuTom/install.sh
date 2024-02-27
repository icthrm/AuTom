if [ -x build ];then
rm -rf build
fi
mkdir build
cd build
cmake ..
make -j2
cp ../runfile/* bin/
chmod 775 bin/*
echo "installation successed!"
