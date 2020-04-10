docker container stop tradingcore2
docker container rm tradingcore2
docker run -d \
    -p 3777:3777 \
    --name tradingcore2 \
    -v $PWD/cfg:/app/tr2/cfg \
    -v $PWD/data:/app/tr2/data \
    tradingcore2