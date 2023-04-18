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

parquet(48 work threads):
```
name    run     timing
benchmark/tpch/parquet/parquet_scan_lineitem_10.benchmark       1       2.424045
benchmark/tpch/parquet/parquet_scan_lineitem_10.benchmark       2       2.028496
benchmark/tpch/parquet/parquet_scan_lineitem_10.benchmark       3       1.556045
benchmark/tpch/parquet/parquet_scan_lineitem_10.benchmark       4       1.666840
benchmark/tpch/parquet/parquet_scan_lineitem_10.benchmark       5       1.755701
```

pixels(48 work threads):
```
benchmark/tpch/pixels/pixels_scan_lineitem_10.benchmark 1       3.972828
benchmark/tpch/pixels/pixels_scan_lineitem_10.benchmark 2       2.552930
benchmark/tpch/pixels/pixels_scan_lineitem_10.benchmark 3       2.403390
benchmark/tpch/pixels/pixels_scan_lineitem_10.benchmark 4       2.625690
benchmark/tpch/pixels/pixels_scan_lineitem_10.benchmark 5       2.660494
```

The reason why parquet is faster than pixels:

* parquet uses filter prune and filter push
* parquet sends the query engine the statistic and cardinality

## 5. encoding 
Now the encoding is disabled. 

## 6. read flag
Should change it to O_DIRECT.
Take care of page cache: https://medium.com/marionete/linux-disk-cache-was-always-there-741bef097e7f

## 7. c++ file open limitation
The limitation of opening file might be 1024 or 512. Should take care of it.

## 8. common issue

* Clion crashes when compiling duckdb + pixels: 
Clion runs a jvm program. The heap size is 2 GB by default. If out of memory
issue occurs, Clion would be killed.

    https://intellij-support.jetbrains.com/hc/en-us/community/posts/207277465-CLion-killed-when-building-symbols
    https://www.jetbrains.com/help/clion/increasing-memory-heap.html