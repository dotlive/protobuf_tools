echo off

if not exist build md build & pushd build
cmake -G "Visual Studio 15 2017" ../
popd

cmake --build build --config Debug

pause
