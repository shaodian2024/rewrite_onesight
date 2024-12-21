cmake -G "Xcode" -DBUILD_FOR_macOS=ON -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" .. 
cmake --build . --config Release