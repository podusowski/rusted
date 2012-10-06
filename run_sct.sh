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
pushd _build

if [ "$tool" = "helgrind" ]; then
    wrapper="valgrind --tool=helgrind --log-file=$root/helgrind-%p --trace-children=yes --child-silent-after-fork=yes"
    export SERVER_SCT_WAIT_FOR_APP_TIME=10
    rm -f $root/helgrind*
fi

$wrapper ./ServerSCT $@

popd

