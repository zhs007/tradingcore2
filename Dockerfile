# https://github.com/zhs007/tradingcore2
# zerrozhao@gmail.com

FROM buildpack-deps:buster as builder

RUN apt-get update -y \
  && apt-get install git wget gcc g++ make automake libssl-dev -y \
  && rm -rf /var/lib/apt/lists/*

RUN wget https://github.com/Kitware/CMake/releases/download/v3.18.5/cmake-3.18.5.tar.gz \
  && tar zxvf cmake-3.* \
  && cd cmake-3.* \
  && ./bootstrap --prefix=/usr/local \
  && make \
  && make install \
  && cd .. \
  && rm -rf cmake-3.*

WORKDIR /app/tr2

COPY ./ ./

RUN sh initdeps.sh
RUN sh build.sh

FROM debian:buster-slim
WORKDIR /app/tr2
COPY --from=builder /app/tr2/build/src/server/tc2serv /app/tr2/
COPY ./data /app/tr2/data
RUN mkdir output
CMD ["./tc2serv", "./cfg/config.yaml"]