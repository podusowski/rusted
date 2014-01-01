#!/bin/bash
cd $__build
zip_file=$__build/RustedClient-$__configuration___name.zip
rm -f $zip_file

files="Contents "

if [ -f Client.exe ]; then
    files+="Client.exe "
fi

if [ -f OIS.dll ]; then
    files+="*.dll "
fi

zip -r $zip_file $files

