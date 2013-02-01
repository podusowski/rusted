green="\033[1;32m"
red="\033[1;31m"
reset="\033[0;m"

tool=""
pattern=".*"

while getopts 't:p:' opt; do
    case $opt in
        t)
            tool=$OPTARG
            ;;
        p)
            pattern=$OPTARG
            ;;
    esac
done

echo tool: $tool
echo pattern: $pattern

root=`pwd`
pushd _build

if [ "$tool" = "helgrind" ]; then
    wrapper="valgrind --tool=helgrind --log-file=$root/helgrind-%p --trace-children=yes --child-silent-after-fork=yes"
    export SERVER_SCT_WAIT_FOR_APP_TIME=10
    rm -f $root/helgrind*
fi

if [ "$tool" = "memcheck" ]; then
    wrapper="valgrind --tool=memcheck --log-file=$root/memcheck-%p --trace-children=yes --child-silent-after-fork=yes"
    export SERVER_SCT_WAIT_FOR_APP_TIME=10
    rm -f $root/memcheck*
fi

function run_single_test()
{
    name=$1
    port=$2
    log_dir=../_build/sct/

    mkdir -p $log_dir

    SERVER_SCT_PORT=$port $wrapper ./ServerSCT --gtest_filter=$name > $log_dir/$name.out 2> $log_dir/$name.err

    if grep FAILED $log_dir/$name.out > /dev/null; then
        result="  ${red}fail$reset "
    else
        result="  ${green}pass$reset "
    fi

    echo -e "$result $name port: $port `grep Failure $log_dir/$name.out`"
}

function read_testcases()
{
    ./ServerSCT --gtest_list_tests | while read line 
    do
        if echo $line | grep "\." > /dev/null; then
            prefix=$line
        else
            echo -n "$prefix$line "
        fi
    done
}

testcases=`read_testcases`
echo `echo $testcases | wc -w` tests

port=3000
for testcase in $testcases; do
    port=$((port+1))
    run_single_test $testcase $port &
        
    while test `jobs | wc -l` -gt 6; do
        sleep 1
    done
done

wait

#$wrapper ./ServerSCT $@

popd

