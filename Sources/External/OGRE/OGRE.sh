mkdir -p $ogredeps_dir
cd $ogredeps_dir
cmake -DCMAKE_BUILD_TYPE=Release $ogredeps_src
make install

