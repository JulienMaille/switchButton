rmdir /s /q build-%PLATFORM%
mkdir build-%Platform%
cd build-%Platform%

cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_DESIGNER_PLUGIN=ON
cmake --build . --clean-first
cmake --build . --target install

cd ..