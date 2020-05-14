#!/bin/sh 

mkdir built

echo "Building kmod"
cd kern
cmake .
make install
mv ./kern_clockset.skprx ../app

make clean
rm -rf ./CMakeFiles
rm -rf ./kern_clockset-stubs
rm -f cmake_install.cmake
rm -f CMakeCache.txt
rm -f Makefile
rm -f install_manifest.txt

cd ..

echo "Building umod"
cd user
cmake .
make install
mv ./user_clockset.suprx ../app

make clean
rm -rf ./stubs
rm -rf ./CMakeFiles
rm -f CMakeCache.txt
rm -f Makefile
rm -f install_manifest.txt
rm -f cmake_install.cmake


cd .. 

echo "Building app"
cd app
cmake . && make 
mv TimeMachine.vpk ../built

make clean
rm -rf ./CMakeFiles
rm -f TimeMachine.velf
rm -f cmake_install.cmake
rm -f CMakeCache.txt
rm -f eboot.bin
rm -f Makefile 
rm -f param.sfo
rm -f kern_clockset.skprx
rm -f user_clockset.suprx