rm -rf ./dockerhub/trnode2
mkdir ./dockerhub/trnode2
docker build -f ./Dockerfile.builddockerhub -t trnode2 .
docker run -d --name trnode2 trnode2 sh -c "while true; do sleep 1; done"
docker cp trnode2:/app/tr2/build/src/server/tc2serv ./dockerhub/trnode2/
docker stop trnode2
docker rm trnode2
cp ./VERSION ./dockerhub/trnode2/
