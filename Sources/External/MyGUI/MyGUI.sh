mkdir -p $mygui_dir
cd $mygui_dir
echo $OGRE_ogredeps_dir
cmake $mygui_src -DMYGUI_RENDERSYSTEM=2 -DOGRE_SOURCE=$OGRE_ogre_src -DOGRE_BUILD=$OGRE_ogre_dir -DMYGUI_DEPENDENCIES_DIR=$OGRE_ogredeps_dir/ogredeps
make MyGUIEngine MyGUI.OgrePlatform
cp -v $mygui_dir/lib/* $__build

