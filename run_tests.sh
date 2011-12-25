#!/bin/bash

if [ ! -d _build ]; then 
    echo "no _build directory, project was not built or you are not in working directory"
    exit 1
fi

function run_test()
{
    test_name=$1

    stdout=`tempfile`
    stderr=`tempfile`
    ./$test_name > $stdout 2> $stderr
    if [ "$?" = "0" ]; then
        echo -n "pass "
    else
        echo -n "fail "
        cat $stdout $stderr
        #result=1
        exit 1
    fi
    echo $test_name

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
