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
if [[ -d "temp" ]]
then
	echo "borrando directorio temp..."
	rm -rf temp
	echo "directorio borrado"

	#crear temp y posicionarse en temp
	mkdir temp && cd temp
fi

#crear ficheros
echo "Hello Word!" >> file1.txt
head /etc/passwd >> file2.txt
head -c 1024 /dev/urandom >> file3.dat

#cd ../ && cp /temp/file1.txt #prueba de copia
cd .. && ./mytar filetar.mtar file1.txt file2.txt file3.dat
cd /tmp && mkdir out
cd out && cp ../filetar.mtar . #lo copia en el directorio actual

