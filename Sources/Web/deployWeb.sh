#!/bin/bash

root_directory=`dirname $0`
web_directory=/var/www/rusted/

function deploy()
{
    if [ ! -e $web_directory ]; then
        echo "web directory doesn't exist, you have to create it before proceeding"
        exit 1
    fi

    cp -v $root_directory/*.php $web_directory
}

deploy

