#!/bin/bash

set -u

green="\033[1;32m"
red="\033[1;31m"
red2="\033[31m"
yellow="\033[1;32m"
bold="\033[1;24m"
reset="\033[0;m"

tool=""
pattern='.*'
binary=""
fast=0

while getopts 't:p:b:f' opt; do
    case $opt in
        t)
            tool=$OPTARG
            ;;
        p)
            pattern=$OPTARG
            ;;
        b)
            binary=$OPTARG
            ;;
        f)
            fast=1
            ;;
    esac
done

function is_tool_supported()
{
    local tool=$1

    if [ "$tool" = "helgrind" -o "$tool" = "memcheck" ]; then
        return 0
    else
        return 1
    fi
}

function run_test_binary()
{
    local binary=$1
    local testcases=`read_testcases $binary`

    port=3000
    for testcase in $testcases; do
        port=$((port+1))

        if [ $fast = "1" ]; then
            ( run_single_test $binary $testcase $port ) &
        else
            ( run_single_test $binary $testcase $port )
        fi

        while test `jobs | wc -l` -gt 6; do
            sleep 1
        done
    done

    wait
}

function run_single_test()
{
    local binary=$1
    local name=$2
    local port=$3

    local log_dir=$PWD/`dirname $binary`/../tests/`basename $binary`/
    local log_file=$log_dir/$name.log
    local binary_dir=$PWD/`dirname $binary`

    mkdir -p $log_dir

    pushd `dirname $binary` > /dev/null

    local wrapper=""
    local tool_log=""
    local tool_output=""
    if is_tool_supported "$tool"; then
        tool_log=$log_dir/$name.$tool
        wrapper="valgrind --xml=yes --tool=$tool --log-file=$tool_log --trace-children=yes --child-silent-after-fork=yes"
        export SERVER_SCT_WAIT_FOR_APP_TIME=10
    fi

    LD_LIBRARY_PATH=. SERVER_SCT_PORT=$port $wrapper ./`basename $binary` --gtest_filter=$name &> $log_file

    if grep FAILED $log_file > /dev/null; then
        result="  ${red}fail$reset "
    else
        result="  ${green}pass$reset "
    fi

    if is_tool_supported "$tool"; then
        tool_output=`grep 'ERROR SUMMARY' $tool_log | grep -v 'ERROR SUMMARY: 0'`
    fi

    local failure_details=`grep Failure $log_file`
    local prefix=""
    if [ -n "$failure_details" ]; then
        failure_details="\n$failure_details\nsee ${red}`readlink -f $log_file`${reset}\n"
        prefix="\n"
    fi

    echo -e "${prefix}$result $bold`basename $binary`$reset.$name ${red2}${failure_details}${reset} ${red}${tool_output}${reset}"

    popd > /dev/null
}

function read_testcases()
{
    local binary=$1

    LD_LIBRARY_PATH=`dirname $binary` $binary --gtest_list_tests | while read line
    do
        if echo $line | grep "\." > /dev/null; then
            prefix=$line
        else
            local name="$prefix$line"

            if [ -n "$pattern" ]; then
                if echo $binary.$name | grep "$pattern" > /dev/null; then
                    echo -n "$name "
                fi
            else
                echo -n "$name "
            fi
        fi
    done
}

function cleanup()
{
    sleep 0.2
    pkill -P `jobs -p` 2> /dev/null > /dev/null

    sleep 0.1
    if [ -n "`jobs -p`" ]; then
        pkill -KILL -P `jobs -p` 2> /dev/null > /dev/null
    fi

    wait
}

if [ "$tool" != "" ]; then
    if ! is_tool_supported $tool; then
        echo unknown tool: $tool
        exit 1
    fi
fi

trap "cleanup" EXIT TERM

if [ -n "$binary" ]; then
    run_test_binary $binary
else
    root_dir=`dirname $0`/_build/linux-debug/
    for b in $root_dir/*UT $root_dir/*SCT; do
        if [ -f $b -a -x $b ]; then
            if [ $fast = "1" ]; then
                run_test_binary $b &
            else
                run_test_binary $b
            fi
        fi
    done

    wait
fi

echo
cleanup
