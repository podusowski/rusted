if [ ! -d _build ]; then 
    echo "no _build directory, project was not built or you are not in working directory"
    exit 1
fi

pushd _build > /dev/null

for i in *UT; do
   if [ -f $i ]; then
       ./$i > /dev/null 2> /dev/null
       if [ "$?" = "0" ]; then
           echo -n "pass "
       else
           echo -n "fail "
       fi
       echo $i
   fi
done

popd > /dev/null
