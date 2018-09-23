#!/bin/bash

file=mytar

#comprobar que existe mytar
if [[ -f "$file" && -x "$file" ]]
then
  echo "existe"
else
  echo "no existe"
fi

#comprobar que existe temp
if [[ -d "tmp" ]]
then
  echo "borrando directorio tmp..."
  rm -rf tmp
  echo "directorio borrado"

  #crear temp y posicionarse en temp
  echo "Creando tmp"
  mkdir tmp && cd tmp
fi

#crear ficheros
echo "Hello Word!" » file1.txt
head /etc/passwd » file2.txt
head -c 1024 /dev/urandom » file3.dat

#cd ../ && cp /temp/file1.txt #prueba de copia
./mytar -c -f mytar a b c
cd /tmp && mkdir out
cd out && cp ../mytar . #lo copia en el directorio actual
