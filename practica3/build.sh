#!/bin/bash

# Compilar el proyecto, aprender las muestras y clasificar los nuevos objetos

cmake -Bbuild
cd build
make
cd -

echo "Aprender"
echo "--------"
./aprender.sh build/aprender ../images/imagenesL3

if [ $? -ne 0 ]; then
    exit 1
fi

for reco in ../images/imagenesL3/reco*; do
    echo
    echo "Reconocer"
    echo "---------"
    build/reconocer $reco
done
