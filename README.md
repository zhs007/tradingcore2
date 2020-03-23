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

# 项目用途

1. 策略训练&回测。
2. 各种indicator的实现。
3. 各种数据的清洗和处理。
4. 通过grpc来做分布运算。
5. 配合Ada做图表。
