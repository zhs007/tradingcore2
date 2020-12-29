rm -rf ./dockerhub/trnode2
mkdir ./dockerhub/trnode2
mkdir build
docker build -f ./Dockerfile.builddockerhub -t trnode2 .
docker run --rm -v $PWD/deps:/app/tr2/deps -v $PWD/build:/app/tr2/build --name trnode2 trnode2 sh -c "sh buildinbuilder.sh"
# docker run -d --name trnode2 trnode2 sh -c "while true; do sleep 1; done"
# docker cp trnode2:/app/tr2/build/src/server/tc2serv ./dockerhub/trnode2/
# docker stop trnode2
# docker rm trnode2
cp $PWD/build/src/server/tc2serv $PWD/dockerhub/trnode2/
cp $PWD/VERSION $PWD/dockerhub/trnode2/
