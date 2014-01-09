#!/bin/bash
cd $__build
zip_file=$__build/Client-$__configuration___name.zip
rm -f $zip_file

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

zip -r $zip_file $files

