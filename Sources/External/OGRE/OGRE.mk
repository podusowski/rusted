TARGET:=OGRE
TYPE:=custom

OGRE_PACKAGE_NAME:=$(TARGET_BASE)/ogre_src_v1-8-1.tar.bz2
OGREDEPS_PACKAGE_NAME:=$(TARGET_BASE)/ogredeps.tar.gz

OGREDEPS_DIR:=$(BUILD)/OGREDEPS
OGRE_DIR:=$(BUILD)/OGRE/ogre_src_v1-8-1/

.PHONY: OGRE OGREDEPS

OGRE: OGREDEPS

OGREDEPS:
	mkdir -p $(OGREDEPS_DIR)
	cd $(OGREDEPS_DIR) && \
		tar xf $(OGREDEPS_PACKAGE_NAME) && \
		cmake . && \
		make

OGRE:
	mkdir -p $(OGRE_DIR)
	cd $(OGRE_DIR)/.. && tar xf $(OGRE_PACKAGE_NAME)
	mkdir -p $(OGRE_DIR)/_build
	cd $(OGRE_DIR)/_build && \
		cmake .. && \
		make OgreMain RenderSystem_GL Plugin_OctreeSceneManager Plugin_ParticleFX

