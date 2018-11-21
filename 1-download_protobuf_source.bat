::参考文章 https://github.com/google/protobuf/blob/master/cmake/README.md
::默认当前操作系统已安装 git 和 cmake, 并配置好了环境变量
echo off & color 0A

::设置所需要的Protobuf版本,最新版本可以在github上查到 https://github.com/google/protobuf
set PROTOBUF_VESION="3.6.x"
echo %PROTOBUF_VESION%
set PROTOBUF_PATH="protobuf_%PROTOBUF_VESION%"
echo %PROTOBUF_PATH%

::从githug上拉取protobuf源代码
git clone -b %PROTOBUF_VESION% https://github.com/google/protobuf.git %PROTOBUF_PATH%

::更新所有子模块
cd %PROTOBUF_PATH%
git submodule update --init --recursive

echo %cd%
pause
