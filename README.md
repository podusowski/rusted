# Rusted, a MMO space game

## Building

Rusted is build by pake build system. To compile all just type:

`./pake.py -a`

to build all targets for the `__default` configuration which is pretty much linux debug.

## Testing

`./run_tests.py` runs everything which is an automated test.

## Dependencies

Following are dependencies from Debian repository, rest are stored under git repository.

```
libx11-dev
libxaw7-dev
libglu1-mesa-dev
libxrandr-dev
libsqlite3-dev
mingw-w64-dev
zip
g++-multilib
```

## External libraries
This software uses following 3rd party libraries:

 * [MyGUI](http://mygui.info/) is released under MIT license
 * Jura font by Daniel Johnson on [SIL Open Font License](http://scripts.sil.org/cms/scripts/page.php?item_id=OFL_web)
 * [OGRE](http://www.ogre3d.org) is made available under the MIT License
 * [boost](http://www.boost.org)