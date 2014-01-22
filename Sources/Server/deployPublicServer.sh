#!/bin/bash

root_directory=`dirname $0`
rusted_directory=/var/rusted/
pid_file=$rusted_directory/Server.pid
log_file=$rusted_directory/Server.log
database_file=$rusted_directory/database.sqlite3
daemon_arguments="-v -d --name=rusted --env=LD_LIBRARY_PATH=$rusted_directory --output=$log_file --pidfile=$pid_file"
administration_socket=$rusted_directory/administrator.socket

function __copy_files()
{
    cp -v $root_directory/Server $rusted_directory
    cp -v $root_directory/libsoci* $rusted_directory
    cp -v $root_directory/AdministrationClient $rusted_directory
}

function __write_log_header()
{
    echo "*** `date` Starting rusted server" >> $log_file
}

function __set_permissions()
{
    echo "setting permissions"
    chmod -v g+w $administration_socket
    chmod -v g+w $database_file
}

function stop()
{
    if [ -f $pid_file ]; then
        echo "stopping server"
        kill `cat $pid_file`
        rm $pid_file || true
        rm $administration_socket || true
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
    local server_arguemnts+="--network.administration_socket_path $administration_socket "
    local server_arguemnts+="--database.url sqlite3://$database_file "

    echo "starting Server with arguments: $server_arguemnts"
    __write_log_header
    daemon $daemon_arguments -- $rusted_directory/Server $server_arguemnts || echo "error in daemon"

    echo "log available in $rusted_directory/Server.log"

    __set_permissions
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

