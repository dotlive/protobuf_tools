::参考文章 https://github.com/google/protobuf/blob/master/cmake/README.md
::默认当前操作系统已安装 git 和 cmake, 并配置好了环境变量
echo off & color 0A

::设置所需要的Protobuf版本,最新版本可以在github上查到 https://github.com/google/protobuf
::必须与下载的版本一致
set PROTOBUF_VESION="3.6.x"
echo %PROTOBUF_VESION%
set PROTOBUF_PATH="protobuf_%PROTOBUF_VESION%"
echo %PROTOBUF_PATH%
set INSTALL_PATH="protoc_%PROTOBUF_VESION%"

::检查安装目录, 必须在切换目录前完成
if not exist %INSTALL_PATH% md %INSTALL_PATH%
del %INSTALL_PATH%\*.* /f /s /q

cd %PROTOBUF_PATH%

::设置VS工具集,相当于指定VS版本,取决于VS的安装路径
if exist "%VS140COMNTOOLS%\VsDevCmd.bat" (
    echo "vs2015"
    set VS_DEV_CMD="%VS140COMNTOOLS%\VsDevCmd.bat"
    set BUILD_PATH="build_VS2015"
    set CMAKE_GENERATOR="Visual Studio 14 2015"
) else (
    echo "vs2017"
    set VS_DEV_CMD="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
    set BUILD_PATH="build_VS2017"
    set CMAKE_GENERATOR="Visual Studio 15 2017"
)
::设置编译版本 Debug Or Release
set MODE="Debug"


cd cmake
if not exist %BUILD_PATH% md %BUILD_PATH%

cd %BUILD_PATH%
if not exist %MODE% md %MODE%
cd %MODE%

::开始构建和编译
call %VS_DEV_CMD%

::生成Makefile
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=%MODE% -DCMAKE_INSTALL_PREFIX=../../../../%INSTALL_PATH% -Dprotobuf_BUILD_TESTS=OFF ../..
::cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=%MODE% -DCMAKE_INSTALL_PREFIX=../../../../%INSTALL_PATH% -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_SHARED_LIBS=ON ../..

::生成.sln
cd ..
if not exist solution md solution
cd solution
cmake -G %CMAKE_GENERATOR% -DCMAKE_INSTALL_PREFIX=../../../../%INSTALL_PATH% -Dprotobuf_BUILD_TESTS=OFF ../..
::cmake -G %CMAKE_GENERATOR% -DCMAKE_INSTALL_PREFIX=../../../../%INSTALL_PATH% -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_SHARED_LIBS=ON ../..

cd ../%MODE%
nmake /f Makefile

::Testing
::nmake check

::Installing
nmake install

echo %cd%
pause
