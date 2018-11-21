# Protobuf工具集

包括 **Protobuf** 源码下载、工具的编译生成、目标语言的生成及功能测试。

---
### Roadmap

- protobuf_XXX

    protobuf 源码所在目录

- protoc

    proto 文件编辑工具及开发依赖资源所在目录，按语言进行子目录划分：

    - cpp
    - csharp
    - java
    - js
    - lua
    - objc
    - php
    - python
    - ruby

- proto

    proto 源文件所在目录。

- output

    proto 目标语言输出目录。按目标语言行输出目录划分：

    - cpp
    - csharp
    - java
    - js
    - lua
    - objc
    - php
    - python
    - ruby

- bat文件

    - 1-download_protobuf_source.bat

        下载指定版本的 protobuf 源码

    - 2-build_protobuf.bat

        编译生成开发资源，包括 protoc, cmake, include, lib

    - 3-generate.bat

        生成 proto 的目标语言源文件


---
### FAQ

1. protobuf 源码仓库改变本地目录后，之前 cmake 生成的工程文件，还是会指向原来的目录，所以需要重新生成；
2. csharp, php, ruby 只支持 proto3 语法；
3. lua 不支持生成到一个文件里面；
