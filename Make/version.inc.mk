CFLAGS+=-DVERSION="null"

GIT_VERSION="`which git && git log | head -n3 | grep "commit\|Date" | xargs echo | sed "s/commit \(.*\) Date: \(.*\)/git:\1 \(\2\)/"`"
