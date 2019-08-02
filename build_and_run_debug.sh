echo "Building with debug flag..."
cd bin
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
cd ..
echo "Running..."
./bin/src/app