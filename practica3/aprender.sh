#!/bin/bash

# Aprender todas las muestras de un directorio

if [ $# -ne 2 ]; then
    echo "Uso: $0 ejecutable directorio"
fi

ejecutable=$1
dir=$2

truncate -s 0 objetos.txt
truncate -s 0 muestras.txt

for clase in triangulo circulo rectangulo rueda vagon; do
    for muestra in "$dir/$clase"*; do
        $ejecutable $muestra $clase
    done
done
