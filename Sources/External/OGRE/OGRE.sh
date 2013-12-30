set -e

if [ ! -f "$artefacts" ]; then
    mkdir -p $ogredeps_dir
    cd $ogredeps_dir
    echo $__configuration_cmake_flags
    cmake $__configuration_cmake_flags -DCMAKE_BUILD_TYPE=Release $ogredeps_src
    make install

    OGRE_CMAKE_OPTIONS="-DOGRE_DEPENDENCIES_DIR=$ogredeps_dir/ogredeps"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS -DOGRE_STATIC=1"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS -DOGRE_BUILD_RENDERSYSTEM_D3D9=0"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS -DOGRE_BUILD_COMPONENT_PAGING=0"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS -DOGRE_BUILD_COMPONENT_PROPERTY=0"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS -DOGRE_BUILD_COMPONENT_RTSHADERSYSTEM=0"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS -DOGRE_BUILD_TOOLS=0"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS -DOGRE_INSTALL_TOOLS=0"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS -DOGRE_BUILD_COMPONENT_TERRAIN=0"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS -DOGRE_BUILD_PLUGIN_PFX=1"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS -DOGRE_BUILD_PLUGIN_PCZ=0"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS -DOGRE_BUILD_PLUGIN_BSP=0"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS -DOGRE_BUILD_SAMPLES=0"
    OGRE_CMAKE_OPTIONS="$OGRE_CMAKE_OPTIONS $__configuration_cmake_flags"

    mkdir -p $ogre_dir
    cd $ogre_dir
    echo cmake -DCMAKE_BUILD_TYPE=MinSizeRel $OGRE_CMAKE_OPTIONS $ogre_src
    cmake -DCMAKE_BUILD_TYPE=MinSizeRel $OGRE_CMAKE_OPTIONS $ogre_src
    make
    cp -rv $ogre_dir/lib/* $__build
    cp -rv $ogre_dir/bin/* $__build
    cp -rv $ogredeps_dir/ogredeps/lib/* $__build

    if [ -e $ogredeps_dir/ogredeps/lib/Release ]; then
        cp -rv $ogredeps_dir/ogredeps/lib/Release/* $__build
    fi

    if [ -e $ogredeps_dir/ogredeps/bin/Release ]; then
        cp -rv $ogredeps_dir/ogredeps/bin/Release/* $__build
    fi

    # hax
    if [ -f $__build/libOIS.dll.a ]; then
        mv $__build/libOIS.dll.a $__build/libOIS.a
    fi
fi

