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

## 4. performance

TPCH 1:

parquet(11 work threads):
```
benchmark/tpch/parquet/parquet_scan_lineitem.benchmark  1       0.422783
benchmark/tpch/parquet/parquet_scan_lineitem.benchmark  2       0.411751
benchmark/tpch/parquet/parquet_scan_lineitem.benchmark  3       0.411907
benchmark/tpch/parquet/parquet_scan_lineitem.benchmark  4       0.319176
benchmark/tpch/parquet/parquet_scan_lineitem.benchmark  5       0.332601
```

pixels(11 work threads):
```
benchmark/tpch/pixels/pixels_scan_lineitem.benchmark    1       0.810392
benchmark/tpch/pixels/pixels_scan_lineitem.benchmark    2       0.825443
benchmark/tpch/pixels/pixels_scan_lineitem.benchmark    3       0.650826
benchmark/tpch/pixels/pixels_scan_lineitem.benchmark    4       0.575485
benchmark/tpch/pixels/pixels_scan_lineitem.benchmark    5       0.582442
```

TPCH 10:

parquet(11 work threads):
```
benchmark/tpch/parquet/parquet_scan_lineitem_10.benchmark       1       4.029747
benchmark/tpch/parquet/parquet_scan_lineitem_10.benchmark       2       3.988435
benchmark/tpch/parquet/parquet_scan_lineitem_10.benchmark       3       4.048495
benchmark/tpch/parquet/parquet_scan_lineitem_10.benchmark       4       3.775438
benchmark/tpch/parquet/parquet_scan_lineitem_10.benchmark       5       3.931187
```
The reason why parquet is faster than pixels:

* parquet uses filter prune and filter push
* parquet sends the query engine the statistic and cardinality

## 5. encoding 
Now the encoding is disabled. 

## 6. common issue

* Clion crashes when compiling duckdb + pixels: 
Clion runs a jvm program. The heap size is 2 GB by default. If out of memory
issue occurs, Clion would be killed.

    https://intellij-support.jetbrains.com/hc/en-us/community/posts/207277465-CLion-killed-when-building-symbols
    https://www.jetbrains.com/help/clion/increasing-memory-heap.html