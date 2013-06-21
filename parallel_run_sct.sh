green="\033[1;32m"
red="\033[1;31m"
reset="\033[0;m"

tool="none"
pattern='.*'
root=`pwd`
log_dir=$root/_build/sct/
build_dir=$root/_build/linux-debug/
binary=ServerSCT

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

echo "Running $binary using $tool"

pushd $build_dir > /dev/null

function run_test_binary()
{
    binary=$1
}

function run_single_test()
{
    name=$1
    port=$2

    mkdir -p $log_dir

    if [ "$tool" = "helgrind"  -o "$tool" = "memcheck" ]; then
        log=$log_dir/$name.$tool
        wrapper="valgrind --tool=$tool --log-file=$log --trace-children=yes --child-silent-after-fork=yes"
        export SERVER_SCT_WAIT_FOR_APP_TIME=10
        tool_output=`grep 'ERROR SUMMARY' $log | grep -v 'ERROR SUMMARY: 0'`
    fi

    SERVER_SCT_PORT=$port $wrapper ./$binary --gtest_filter=$name > $log_dir/$name.out 2> $log_dir/$name.err

    if grep FAILED $log_dir/$name.out > /dev/null; then
        result="  ${red}fail$reset "
    else
        result="  ${green}pass$reset "
    fi

    echo -e "$result $name `grep Failure $log_dir/$name.out`" ${red}${tool_output}${reset}
}

function read_testcases()
{
    ./$binary --gtest_list_tests | while read line 
    do
        if echo $line | grep "\." > /dev/null; then
            prefix=$line
        else
            echo -n "$prefix$line "
        fi
    done
}

function cleanup()
{
    sleep 0.5
    kill `jobs -p` 2> /dev/null

    sleep 0.3
    if [ -n "`jobs -p`" ]; then
        kill -KILL `jobs -p` 2> /dev/null
    fi
}

trap "cleanup" EXIT

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

cleanup
