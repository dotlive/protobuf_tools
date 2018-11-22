echo off

if not exist build md build & pushd build
cmake -G "Visual Studio 14 2015" ../
popd

cmake --build build --config Debug

pause
