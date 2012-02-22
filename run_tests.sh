#!/bin/bash

FONT_COLOR_GREEN="\033[1;32m"
FONT_COLOR_RED="\033[1;31m"
FONT_RESET="\033[0;m"

verbose=0
compile=0

while getopts 'vc' opt; do
    case $opt in
        v)
            verbose=1
            ;;
        c)
            compile=1
            ;;
    esac
done

if [ $compile = "1" ]; then
    make all
fi

if [ ! -d _build ]; then 
    echo "no _build directory, project was not built or you are not in working directory"
    exit 1
fi

function run_test()
{
    test_name=$1

    stdout=`tempfile`
    stderr=`tempfile`

    if [ $verbose = "1" ]; then
        LD_LIBRARY_PATH=. ./$test_name
    else
        LD_LIBRARY_PATH=. ./$test_name > $stdout 2> $stderr
    fi

    if [ "$?" = "0" ]; then
        echo -en "${FONT_COLOR_GREEN}pass$FONT_RESET $test_name"
    else
        echo -e "${FONT_COLOR_RED}fail$FONT_RESET $test_name"
        if [ $verbose != "1" ]; then
            cat $stdout $stderr
        fi
        echo "no further tests will be executed"
        exit 1
    fi
    echo 

    rm $stdout $stderr
}

pushd _build > /dev/null

result=0

for i in *UT; do
   if [ -f $i ]; then
       run_test $i
   fi
done

for i in *SCT; do
   if [ -f $i ]; then
       run_test $i
   fi
done

popd > /dev/null

exit $result
