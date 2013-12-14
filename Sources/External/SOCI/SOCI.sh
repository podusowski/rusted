SOCI_PACKAGE_NAME=soci-3.2.1.tar.gz
SOCI_SRC=$__path/soci-3.2.1/
SOCI_DIR=$__build/SOCI/
SOCI_ARTEFACT=$__build/libsoci_core.so.3.2.1

mkdir -p $SOCI_DIR
cd $__build/SOCI
cmake $SOCI_SRC
make
cp -v $SOCI_DIR/lib/* $__build

