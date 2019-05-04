#! /bin/bash

cd TrialDivision
/bin/bash ./build.sh
cp -a ./bin/Linux_Release/. ../Tester

cd ../Rho
make
cp ./thread_rho ../Tester/thread_rho

cd ../QuadraticSieve
/bin/bash ./build.sh
cp -a ./bin/Linux_Release/. ../Tester