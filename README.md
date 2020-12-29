# TradingCore2

这是一个轻量级分布式交易内核项目。  

服务稳定，适合长时间多节点运算。  
10年日数据级别的策略运算（远程拉取数据+各种Indicator实时计算），单核基本上1s内能完成。  
如果策略复杂，十万甚至百万级别的任务量，耗时几乎都是用天来计算了（十万级单核就要1天多了），如果没有分布式或离线运算，将是一件非常痛苦的事情。
在线上，有一套近60核的环境已经稳定运行数月。  

C++做运算，Golang做分发，Python做分析。

之所以用c++写，除了效率考虑外，主要是因为后期扩展方便（可以很方便的给别的语言用）。  

至于轻量级，是因为尽可能的少写了框架性代码，更多的精力放在功能上。  
然后，就是避免大量使用template了（只在必须用的时候才用，其实还是懒......）。  
不为不必要的需求买单。

作为交易内核，主要用途其实就3个：

1. 底层算法实现，包括各种indicator、各种数据转换、数据清洗等。
2. 策略分析用的计算节点，用于分布式训练策略。  
默认情况下会将节点cpu占满，建议``tasknums``配置为-1，这样会保留一个cpu内核出来。  
如果运算节点对cpu占用有较高要求（不允许长时间满负载运行），还可以配置``tasktimeoff``，这个是任意2个task之间最小时间间隔，单位为ms，可以自行尝试合适的配置。  
如果外网节点，希望防止运算节点被别人滥用，可以配置有效``tokens``。
3. 实盘运算节点，支持分布式运算，提供实盘交易信号。

# Build & Test

不管是不是开发环境，都需要先初始化依赖。

``` bash
git submodule init
git submodule update --recursive
git submodule update --init --recursive

sh buildgoogletest.sh

sh buildgsl.sh

sh fixgrpc.sh

sh buildgrpc.sh

sh buildglog.sh

sh buildgflags.sh

sh buildrocksdb.sh

sh buildyamlcpp.sh
```

也可以直接执行 ``sh initdeps.sh`` 。

本项目编译方法如下：

``` bash
mkdir build
cd build
cmake ..
make
```

编译完成后，可以执行测试用例（直接在build目录下执行即可）。

``` bash
./tests/tests
```

# Docker

如果你只想部署训练节点，可以看[这里](https://github.com/zhs007/dockerscripts/tree/master/tc2)。  
这是一个用于快速部署的脚本，不需要编译，镜像大小仅27MB。  
且自动同步master分支最新版（``dockerhub``的build非常慢，和``github``代码发布大概有1小时以上的延迟）。

除非是开发环境，否则我们建议直接使用docker来部署。  
所以我们提供了Dockerfile，可以一键编译部署。

```
docker build -t tradingcore2 .
```

因为这是一个完整的编译环境，所以依赖较多，编译过程较长。  
但最终build好以后，是个server的运行版，容量较小（27MB）。

除了普通的dockerfile外，我们还提供了：

- dev版，预装了``valgrind``，可以很方便的调试内存泄露等。  

``` sh
valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./tradingcore2
```

- gtest版，这个会自动执行单元测试，我们项目的``github``的``workflows``会执行。

# 项目用途

1. 策略训练&回测。
2. 各种indicator的实现。
3. 各种数据的清洗和处理。
4. 通过``grpc``来做分布运算。
5. 配合``Ada``做图表。
6. 通过``tradingdb2``读写数据，维护数据。
7. 配合``Jupyter``做策略分析。

# TradingDB2

v0.3版开始，数据读取支持 ``tradingdb2`` 。
配置如下：

``` yaml
trdb2serv: 127.0.0.1:5002
trdb2token: 1234567
```

# 注意事项

- 不要直接下载代码包来build，需要``git clone``才行，因为有一些``git submodule``，代码包缺少信息，会``build``失败。
- ``zlib`` 项目如果手动 ``build`` 时，会删除一个文件，所以很容易在项目提交时，造成 ``submodule dirty`` 。
- ``rocksdb`` 项目有很多依赖需要手动装，现在全部添加了 ``submodule`` ，不用手动装官方install文档里的那些依赖了。
- ``cmake`` 对link项目顺序有要求，要先add直接的依赖项目，再add依赖的依赖。
- 基于 ``buildpack-deps:stretch`` 编译出来的项目，不能放在 ``alpine`` 里运行，可以放 ``debian:stretch-slim`` 里。
- 感觉在mac下或linux下，编译时都没办法用到多核，所以编译很慢。
- 修改某些配置后，``cmake`` 的 ``submodule`` 会重新编译，如果大量的依赖库也要重新编译的话，就非常慢了，所以后期调整了策略，依赖库尽可能放 ``initdeps.sh`` 里编译。
- ``grpc``的依赖非常多，最麻烦的是 ``absl`` 还是分目录的库，估计有几十个，如果不是直接``cmake``的``submodule``的话，很多依赖得自己一个个加，很折腾。
- ``dockerhub``编译本项目会报错，直接拿``Dockerfile``编译都会报错，但在``github``和本地环境或其它云服务器上编译都没问题，复测一遍需要1个多小时，所以最终放弃在``dockerhub``上编译发布了。

# 更新日志

### v0.6

- 支持实盘策略。

### v0.5

- 重新调整``grpc``协议，和 ``tradingdb2`` 进一步融合。
- 新的运算节点架构 ``tradingnode2``。
- 结构大幅重构，基本可以通过参数配置出各种复杂策略。
- 支持定投策略。
- 支持集群配合``tradingdb2``，可使用``jupyter``直接研究策略。

### v0.3

- 支持基金的数据计算。
- 支持``tradingdb2``数据源。

### v0.2

- 优化编译流程，提升开发环境下编译速度。

### v0.1

- 完成基本结构。
- 能进行分布式的训练。