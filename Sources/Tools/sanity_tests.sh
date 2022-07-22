#! /usr/bin/env bash
set -euo pipefail
set -x
die() {
    echo $@ 1>&2
    exit 1
}

[ $# -eq 1 ] || die "provide build dir as argument"

readonly SCRIPT_DIR=$(dirname "$0")
readonly REPO_ROOT=$(readlink -f ${SCRIPT_DIR}/../..)
readonly BUILD_DIR=$1
readonly TEMP_DIR="/tmp/rusted/sanity_tests_dir"
readonly CFG_FILE="${TEMP_DIR}/Server.cfg"
readonly DATABASE_FILE="${TEMP_DIR}/TestDataBase.sqlite3"
[ -d "${BUILD_DIR}" ] || die "invalid build dir: ${BUILD_DIR}"

mkdir -p "${TEMP_DIR}"
rm -rf "${TEMP_DIR}"/* #fairly safe as long as `set u` is in place

cat <<EOF >${CFG_FILE}
network.port=1234
network.administration_socket_path=${TEMP_DIR}/rusted.socket
database.url=sqlite3://${DATABASE_FILE}
EOF
sqlite3 ${DATABASE_FILE} <${REPO_ROOT}/Sources/Server/DataBaseSchema.sql
sqlite3 ${DATABASE_FILE} <${REPO_ROOT}/Sources/Server/SCT/SctDataBase.sql

sleep 5
${BUILD_DIR}/Sources/Server/server --cfg "${CFG_FILE}" &
SERVER_PID=$!
echo ${SERVER_PID}
ps aux | grep Server
sleep 2
kill -s SIGINT $SERVER_PID
