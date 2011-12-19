#!/bin/bash

if [ ! -d _build ]; then 
    echo "no _build directory, project was not built or you are not in working directory"
    exit 1
fi

pushd _build > /dev/null

result=0

for i in *UT; do
   if [ -f $i ]; then
       stdout=`tempfile`
       stderr=`tempfile`
       ./$i > $stdout 2> $stderr
       if [ "$?" = "0" ]; then
           echo -n "pass "
       else
           echo -n "fail "
           cat $stdout $stderr
           result=1
       fi
       echo $i

       rm $stdout $stderr
   fi
done

popd > /dev/null

exit $result
