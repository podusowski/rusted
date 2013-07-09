tool=""
while getopts 't:' opt; do
    case $opt in
        t)
            tool=$OPTARG
            ;;
    esac
done

echo tool: $tool

root=`pwd`
pushd _build/linux-debug

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

export LD_LIBRARY_PATH=.
$wrapper ./ServerSCT $@

popd

