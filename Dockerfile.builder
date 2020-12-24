# https://github.com/zhs007/tradingcore2
# zerrozhao@gmail.com

FROM buildpack-deps:buster

RUN apt-get update -y \
  && apt-get install git wget gcc make automake -y \
  && rm -rf /var/lib/apt/lists/*

RUN wget https://github.com/Kitware/CMake/releases/download/v3.18.5/cmake-3.18.5.tar.gz \
  && tar zxvf cmake-3.* \
  && cd cmake-3.* \
  && ./bootstrap --prefix=/usr/local \
  && make \
  && make install \
  && cd .. \
  && rm -rf cmake-3.*

WORKDIR /app/proj