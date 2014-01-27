#!/bin/bash

cd $__build

files="Contents "

if [ -f Client.exe ]; then
    files+="Client.exe "
else
    files+="Client "
fi

if [ -f OIS.dll ]; then
    files+="*.dll "
fi

if [ -f libOIS.so ]; then
    files+="libOIS.so "
fi

files+="config.cfg "

zip_file=$__build/Client-$__configuration___name.zip
echo $zip_file

if [ ! -e "$zip_file" -o `find $files -newer "$zip_file" | wc -l` -gt 0 ]; then
    rm -f $zip_file
    zip -r "$zip_file" $files
else
    echo $zip_file is up to date
fi

