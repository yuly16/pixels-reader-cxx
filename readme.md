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

