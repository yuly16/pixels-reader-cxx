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

TPCH 300:
pixels (without O_DIRECT, HDD)
```
liyu@diascld31:/scratch/liyu/opt/duckdb$ build/release/benchmark/benchmark_runner "benchmark/tpch/pixels/tpch_300/.*"
name    run     timing
benchmark/tpch/pixels/tpch_300/pixels_q10_tpch_300.benchmark    0       187.559770
benchmark/tpch/pixels/tpch_300/pixels_q11_tpch_300.benchmark    0       13.188931
benchmark/tpch/pixels/tpch_300/pixels_q12_tpch_300.benchmark    0       154.773876
benchmark/tpch/pixels/tpch_300/pixels_q13_tpch_300.benchmark    0       386.475201
benchmark/tpch/pixels/tpch_300/pixels_q14_tpch_300.benchmark    0       322.538338
benchmark/tpch/pixels/tpch_300/pixels_q15_tpch_300.benchmark    0       83.861078
benchmark/tpch/pixels/tpch_300/pixels_q16_tpch_300.benchmark    0       14.351079
benchmark/tpch/pixels/tpch_300/pixels_q17_tpch_300.benchmark    0       271.531464
benchmark/tpch/pixels/tpch_300/pixels_q18_tpch_300.benchmark    0       338.800284
benchmark/tpch/pixels/tpch_300/pixels_q19_tpch_300.benchmark    0       1473.391338
benchmark/tpch/pixels/tpch_300/pixels_q1_tpch_300.benchmark     0       395.799622
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

## 9. test case

TPCH 300 pixels hdd:

Q1:
```sql
SELECT
    l_returnflag,
    l_linestatus,
    sum(l_quantity) AS sum_qty,
    sum(l_extendedprice) AS sum_base_price,
    sum(l_extendedprice * (1 - l_discount)) AS sum_disc_price,
    sum(l_extendedprice * (1 - l_discount) * (1 + l_tax)) AS sum_charge,
    avg(l_quantity) AS avg_qty,
    avg(l_extendedprice) AS avg_price,
    avg(l_discount) AS avg_disc,
    count(*) AS count_order
FROM
    '/scratch/liyu/opt/pixels_file/pixels-tpch-300/lineitem/v-0-order/*.pxl'
WHERE
    l_shipdate <= CAST('1998-09-02' AS date)
GROUP BY
    l_returnflag,
    l_linestatus
ORDER BY
    l_returnflag,
    l_linestatus;
```

Q2:

```sql
SELECT
  s_acctbal,
  s_name,
  n_name,
  p_partkey,
  p_mfgr,
  s_address,
  s_phone,
  s_comment
FROM
  '/scratch/liyu/opt/pixels_file/pixels-tpch-300/part/v-0-order/*.pxl',
  '/scratch/liyu/opt/pixels_file/pixels-tpch-300/supplier/v-0-order/*.pxl',
  '/scratch/liyu/opt/pixels_file/pixels-tpch-300/partsupp/v-0-order/*.pxl',
  '/scratch/liyu/opt/pixels_file/pixels-tpch-300/nation/v-0-order/*.pxl',
  '/scratch/liyu/opt/pixels_file/pixels-tpch-300/region/v-0-order/*.pxl'
WHERE
  p_partkey = ps_partkey
  AND s_suppkey = ps_suppkey
  AND p_size = 15
  AND p_type LIKE '%BRASS'
  AND s_nationkey = n_nationkey
  AND n_regionkey = r_regionkey
  AND r_name = 'EUROPE'
  AND ps_supplycost = (
  SELECT
  min(ps_supplycost)
  FROM
  '/scratch/liyu/opt/pixels_file/pixels-tpch-300/supplier/v-0-order/*.pxl',
  '/scratch/liyu/opt/pixels_file/pixels-tpch-300/partsupp/v-0-order/*.pxl',
  '/scratch/liyu/opt/pixels_file/pixels-tpch-300/nation/v-0-order/*.pxl',
  '/scratch/liyu/opt/pixels_file/pixels-tpch-300/region/v-0-order/*.pxl'
  WHERE
  p_partkey = ps_partkey
  AND s_suppkey = ps_suppkey
  AND s_nationkey = n_nationkey
  AND n_regionkey = r_regionkey
  AND r_name = 'EUROPE')
ORDER BY
  s_acctbal DESC,
  n_name,
  s_name,
  p_partkey
  LIMIT 100;
```

TPCH 300 parquet hdd:


Q1:

```sql
SELECT
    l_returnflag,
    l_linestatus,
    sum(l_quantity) AS sum_qty,
    sum(l_extendedprice) AS sum_base_price,
    sum(l_extendedprice * (1 - l_discount)) AS sum_disc_price,
    sum(l_extendedprice * (1 - l_discount) * (1 + l_tax)) AS sum_charge,
    avg(l_quantity) AS avg_qty,
    avg(l_extendedprice) AS avg_price,
    avg(l_discount) AS avg_disc,
    count(*) AS count_order
FROM
    '/scratch/liyu/opt/parquet_file/tpch_300/lineitem.parquet'
WHERE
    l_shipdate <= CAST('1998-09-02' AS date)
GROUP BY
    l_returnflag,
    l_linestatus
ORDER BY
    l_returnflag,
    l_linestatus;
```

Q2:

```sql
SELECT
    s_acctbal,
    s_name,
    n_name,
    p_partkey,
    p_mfgr,
    s_address,
    s_phone,
    s_comment
FROM
    '/scratch/liyu/opt/parquet_file/tpch_300/part.parquet',
    '/scratch/liyu/opt/parquet_file/tpch_300/supplier.parquet',
    '/scratch/liyu/opt/parquet_file/tpch_300/partsupp.parquet',
    '/scratch/liyu/opt/parquet_file/tpch_300/nation.parquet',
    '/scratch/liyu/opt/parquet_file/tpch_300/region.parquet'
WHERE
    p_partkey = ps_partkey
    AND s_suppkey = ps_suppkey
    AND p_size = 15
    AND p_type LIKE '%BRASS'
    AND s_nationkey = n_nationkey
    AND n_regionkey = r_regionkey
    AND r_name = 'EUROPE'
    AND ps_supplycost = (
        SELECT
            min(ps_supplycost)
        FROM
    '/scratch/liyu/opt/parquet_file/tpch_300/supplier.parquet',
    '/scratch/liyu/opt/parquet_file/tpch_300/partsupp.parquet',
    '/scratch/liyu/opt/parquet_file/tpch_300/nation.parquet',
    '/scratch/liyu/opt/parquet_file/tpch_300/region.parquet'
        WHERE
            p_partkey = ps_partkey
            AND s_suppkey = ps_suppkey
            AND s_nationkey = n_nationkey
            AND n_regionkey = r_regionkey
            AND r_name = 'EUROPE')
ORDER BY
    s_acctbal DESC,
    n_name,
    s_name,
    p_partkey
LIMIT 100;

```

10. flamegraph command

```
perf record -a -g -e cpu-cycles -F 400 build/release/benchmark/benchmark_runner benchmark/tpch/parquet/parquet_scan_orders_HDD.benchmark
perf script | inferno-collapse-perf > stacks.folded
cat stacks.folded | inferno-flamegraph > flamegraph.svg
```