#!/bin/bash

root_directory=`dirname $0`
rusted_directory=/var/rusted/
pid_file=$rusted_directory/Server.pid
database_file=$rusted_directory/database.sqlite3
daemon_arguments="-v -d --name=rusted --env=LD_LIBRARY_PATH=$rusted_directory --output=$rusted_directory/Server.log --pidfile=$pid_file"

function __copy_files()
{
    cp -v $root_directory/Server $rusted_directory
    cp -v $root_directory/libsoci* $rusted_directory
    cp -v $root_directory/AdministrationClient $rusted_directory
}

function stop()
{
    if [ -f $pid_file ]; then
        echo "stopping server"
        kill `cat $pid_file`
        rm $pid_file || true
        sleep 1
    else
        echo "server is not running"
    fi
}

function deploy()
{
    if [ ! -e $rusted_directory ]; then
        echo "$rusted_directory doesn't exists and I cannot create it for you :("
        exit 1
    fi

    if [ -f $pid_file ]; then
        echo "pid file exists, cannot start the server"
        exit 1
    fi

    __copy_files

    echo "generating database"
    if [ -f $database_file ]; then
        echo "database already exists"
    else
        database_schema=$root_directory/DataBaseSchema.sql
        cat $database_schema | sqlite3 $database_file
    fi

    echo "applying ships"
    database_ships=$root_directory/DataBaseShips.sql
    cat $database_ships | sqlite3 $database_file
}

function start()
{
    if daemon $daemon_arguments --running ; then
        echo "already running"
        exit 1
    fi

    local server_arguemnts+="--network.port 1987 "
    local server_arguemnts+="--network.administration_socket_path $rusted_directory/administrator.socket "
    local server_arguemnts+="--database.url sqlite3://$database_file "

    echo "starting Server with arguments: $server_arguemnts"
    daemon $daemon_arguments -- $rusted_directory/Server $server_arguemnts || echo "error in daemon"

    echo "log available in $rusted_directory/Server.log"
}

function status()
{
    echo "showing status, to do some other thing use: deploy, start or stop"

    if [ -e $pid_file ]; then
        ps `cat $pid_file`
    else
        echo server is not running
    fi
}

action=$1

case "$action" in
    start) start ;;
    stop) stop ;;
    deploy) deploy ;;
    *) status ;;
esac

