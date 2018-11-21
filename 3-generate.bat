@echo off

::协议文件路径, 最后不要跟“\”符号
set PROTOBUF_VESION="3.6.x"
set SOURCE_FOLDER=%cd%\proto
set TARGET_FOLDER=%cd%\output
set INSTALL_PATH="protoc_%PROTOBUF_VESION%"


::cpp
call:generate_target_source cpp cpp_out
::csharp
call:generate_target_source csharp csharp_out
::java
call:generate_target_source java java_out
::js
call:generate_target_source js js_out
::lua
call:generate_target_source_bytes lua 
::object c
call:generate_target_source objc objc_out
::php
call:generate_target_source php php_out
::python
call:generate_target_source python python_out
::ruby
call:generate_target_source ruby ruby_out


echo Generate success.
pause


::生成目标文件, 需要指定输出目录和输出标识符（函数定义需要放在主体之后）
:generate_target_source
    echo generate_target_source enter
    setlocal
        ::输入参数检查
        set LANGUAGE=%1
        if %LANGUAGE% == "" (
            echo Output path is invalid
            goto:eof
        )
        set OUT_KEY=%2
        if %OUT_KEY% == "" (
            echo Outkey is invalid
            goto:eof
        )
        echo LANGUAGE:%LANGUAGE%
        echo OUT_KEY:%OUT_KEY%
        ::pause

        ::设置局部变量
        set COMPILER_PATH=%cd%\%INSTALL_PATH%\bin\protoc.exe
        echo COMPILER_PATH:%COMPILER_PATH%
        set TARGET_PATH=%TARGET_FOLDER%\%LANGUAGE%
        echo TARGET_PATH:%TARGET_PATH%
        ::pause

        ::输出目录检查
        if not exist %TARGET_PATH% md %TARGET_PATH%
        del %TARGET_PATH%\*.* /f /s /q

        ::遍历所有proto文件 (输出没有保持子目录结构)
        for /r %SOURCE_FOLDER% %%i in ("*.proto") do (
            echo %COMPILER_PATH% --proto_path=%%~dpi --%OUT_KEY%=%TARGET_PATH% %%~nxi
            %COMPILER_PATH% --proto_path=%%~dpi --%OUT_KEY%=%TARGET_PATH% %%~nxi
        )
    endlocal
    echo generate_target_source exit
goto:eof

::生成二进制文件，只需要指定输出目录即可
:generate_target_source_bytes
    echo generate_target_source_bytes enter
    setlocal
        ::输入参数检查
        set LANGUAGE=%1
        if %LANGUAGE% == "" (
            echo Outkey is invalid
            goto:eof
        )
        echo LANGUAGE:%LANGUAGE%
        ::pause

        ::设置局部变量
        set COMPILER_PATH=%cd%\%INSTALL_PATH%\bin\protoc.exe
        echo COMPILER_PATH:%COMPILER_PATH%
        set TARGET_PATH=%TARGET_FOLDER%\%LANGUAGE%
        echo TARGET_PATH:%TARGET_PATH%
        ::pause

        ::输出目录检查
        if not exist %TARGET_PATH% md %TARGET_PATH%
        del %TARGET_PATH%\*.* /f /s /q

        ::遍历所有proto文件 (输出没有保持子目录结构)
        for /r %SOURCE_FOLDER% %%i in ("*.proto") do (
            echo %COMPILER_PATH% --proto_path=%%~dpi -o %TARGET_PATH%\%%~nxi.bytes %%~nxi
            %COMPILER_PATH% --proto_path=%%~dpi -o %TARGET_PATH%\%%~nxi.bytes %%~nxi
        )
    endlocal
    echo generate_target_source_bytes exit
goto:eof

::计算字符串长度
:strlen <stringVarName> [retvar]
    setlocal EnableDelayedExpansion 
    set str=%1
    set num=0
    :label
    set /a num+=1
    set str=%str:~0,-1%
    if defined str goto :label
    endlocal & set %2=%num%
goto:eof
