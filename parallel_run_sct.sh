green="\033[1;32m"
red="\033[1;31m"
reset="\033[0;m"

tool=""
pattern='.*'
root=`pwd`
log_dir=$root/_build/sct/

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

echo "Running ServerSCT"
echo tool: $tool

pushd _build > /dev/null

function run_single_test()
{
    name=$1
    port=$2

    mkdir -p $log_dir

    if [ "$tool" = "helgrind" ]; then
        wrapper="valgrind --tool=helgrind --log-file=$log_dir/$name.helgrind --trace-children=yes --child-silent-after-fork=yes"
        export SERVER_SCT_WAIT_FOR_APP_TIME=10
    fi

    if [ "$tool" = "memcheck" ]; then
        wrapper="valgrind --tool=memcheck --log-file=$log_dir/$name.emcheck --trace-children=yes --child-silent-after-fork=yes"
        export SERVER_SCT_WAIT_FOR_APP_TIME=10
    fi

    SERVER_SCT_PORT=$port $wrapper ./ServerSCT --gtest_filter=$name > $log_dir/$name.out 2> $log_dir/$name.err

    if grep FAILED $log_dir/$name.out > /dev/null; then
        result="  ${red}fail$reset "
    else
        result="  ${green}pass$reset "
    fi

    echo -e "$result $name `grep Failure $log_dir/$name.out`"
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

popd > /dev/null

