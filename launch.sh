#!/bin/bash
docker run -d -p 8080:8080 -p 47269:47269/udp teleplot 2>/dev/null && echo "--- Launch the docker ---" || echo "--- The docker is already launch ---"
cd server
if [[ $1 == "-c" || $1 == "--clean" ]]; then
    echo "--- Clean the server ---"
    make clean
fi
echo "--- Make the server ---"
make
echo "--- Launch the server ---"
./main

