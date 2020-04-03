# TradingCore2

这是一个轻量级交易内核项目。  

这算是我写的第5个交易类项目了，之所以用c++写，除了效率考虑外，主要是因为后期扩展方便（可以很方便的给别的语言用）。  

至于轻量级，是因为尽可能的少写了框架性代码，更多的精力放在功能上。  
然后，就是不用template了。  
不为不必要的需求买单。

# Build & Test

所有依赖都通过submodule加入，有些依赖的cmake脚本没写好，不想手动改，所以会需要单独编译一些依赖。

``` bash
git submodule init
git submodule update --recursive

cd deps/gsl
mkdir build
cd build
cmake ..
make
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

除非是开发环境，否则我们建议直接使用docker来部署。  
所以我们提供了Dockerfile，可以一键编译部署。

```
docker build -t tradingcore2 .
```

因为这是一个完整的编译环境，所以依赖较多，编译过程较长。  
也可以自己制作二进制包。

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
4. 通过grpc来做分布运算。
5. 配合Ada做图表。

# 注意事项

- zlib 项目如果手动 build 时，会删除一个文件，所以很容易在项目提交时，造成 submodule dirty 。
- rocksdb 项目有很多依赖需要手动装，现在全部添加了 submodule ，对项目有少许修改。