# pixels-reader development log

## 1. install prerequisite library

tbb install:
```shell
echo "deb http://cz.archive.ubuntu.com/ubuntu eoan main universe" | sudo tee -a  /etc/apt/sources.list
sudo apt update
sudo apt install libtbb-dev
```

protobuf install:
```shell
apt install protobuf-compiler
```

## 2. endianness

LongColumnVector and IntColumnVector use big endian, while 
DecimalColumnVector and StringColumnVector use small endian. 

For `long`, we read it by big endian. Search for `__builtin_bswap64`
to check who uses small endian. For `int`, we read it by small endian. 

## 3. alignment

StringColumn is not aligned(e.g. reading length in string column). We should fix it later

## 4. common issue

* Clion crashes when compiling duckdb + pixels: 
Clion runs a jvm program. The heap size is 2 GB by default. If out of memory
issue occurs, Clion would be killed.

    https://intellij-support.jetbrains.com/hc/en-us/community/posts/207277465-CLion-killed-when-building-symbols
    https://www.jetbrains.com/help/clion/increasing-memory-heap.html