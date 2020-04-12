FROM buildpack-deps:stretch as builder

MAINTAINER zerro "zerrozhao@gmail.com"

RUN apt-get update -y \
  && apt-get install git wget gcc make automake -y \
  && rm -rf /var/lib/apt/lists/*

RUN wget https://github.com/Kitware/CMake/releases/download/v3.17.0/cmake-3.17.0.tar.gz \
  && tar zxvf cmake-3.* \
  && cd cmake-3.* \
  && ./bootstrap --prefix=/usr/local \
  && make -j$(nproc) \
  && make install \
  && cd .. \
  && rm -rf cmake-3.*

WORKDIR /app/tr2

COPY ./ ./

RUN sh initdeps.sh \
  && sh build.sh

FROM debian:stretch-slim
WORKDIR /app/tr2
COPY --from=builder /app/tr2/build/src/server/tc2serv /app/tr2/
COPY ./data /app/tr2/data
RUN mkdir output
CMD ["./tc2serv", "./cfg/config.yaml"]