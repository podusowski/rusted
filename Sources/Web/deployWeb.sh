#!/bin/bash

set -e

root_directory=`dirname $0`
web_directory=/var/www/rusted/

function syntax_check()
{
    find . -name '*.php' | while read file; do
        php --syntax-check $file
    done
}

function deploy()
{
    if [ ! -e $web_directory ]; then
        echo "web directory doesn't exist, you have to create it before proceeding"
        exit 1
    fi

    cp -rv $root_directory/Page/* $web_directory
}

syntax_check
deploy

