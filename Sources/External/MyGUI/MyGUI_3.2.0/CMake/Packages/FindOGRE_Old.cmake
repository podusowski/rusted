# Find OGRE includes and library
#
# This module defines
#  OGRE_INCLUDE_DIR
#  OGRE_LIBRARIES, the libraries to link against to use OGRE.
#  OGRE_LIB_DIR, the location of the libraries
#  OGRE_FOUND, If false, do not try to use OGRE

include(FindPkgMacros)

# Try to find framework first on Mac OS X
if (APPLE)
	find_framework(OGRE)
endif ()

if (NOT OGRE_FOUND)
# Then try everything else

	if (MYGUI_STANDALONE_BUILD)
		CMAKE_POLICY(PUSH)
		SET(OGRE_INCLUDE_DIR "C:/MYGUIHACK OGRE_HOME MYGUIBRACKETHACK/include" "C:/MYGUIHACK OGRE_SRC MYGUIBRACKETHACK/OgreMain/include" "C:/MYGUIHACK OGRE_HOME MYGUIBRACKETHACK/include/OGRE" "C:/MYGUIHACK OGRE_HOME MYGUIBRACKETHACK/boost_1_42" CACHE STRING "")
		SET(OGRE_LIBRARIES "debug;OgreMain_d;optimized;OgreMain" CACHE STRING "")
		SET(OGRE_LIB_DIR "C:/MYGUIHACK OGRE_HOME MYGUIBRACKETHACK/lib" "C:/MYGUIHACK OGRE_SRC MYGUIBRACKETHACK/lib" "C:/MYGUIHACK OGRE_HOME MYGUIBRACKETHACK/boost_1_42/lib" CACHE STRING "")
		SET(OGRE_FOUND TRUE)
		CMAKE_POLICY(POP)
	else()


        IF (NOT OGRE_SOURCE)
            set(OGRE_SOURCE "" CACHE PATH "Path to Ogre sources (set it if you don't have OGRE_HOME or OGRE_SRC environment variables)")
        ENDIF ()

        if (NOT OGRE_BUILD)
            set(OGRE_BUILD ${OGRE_SOURCE} CACHE PATH "Path to Ogre build directory (same as OGRE_SOURCE by default)")
        endif ()


		# now trying to find it by our script
		CMAKE_POLICY(PUSH)

		IF (OGRE_LIBRARIES AND OGRE_INCLUDE_DIR)
			SET(OGRE_FIND_QUIETLY TRUE) # Already in cache, be silent
		ENDIF (OGRE_LIBRARIES AND OGRE_INCLUDE_DIR)


        MESSAGE(STATUS "Using OGRE built from source (from specified path)")
        SET(OGRE_INCLUDE_DIR ${OGRE_SOURCE}/OgreMain/include ${OGRE_BUILD}/include)
        SET(OGRE_LIB_DIR ${OGRE_BUILD}/lib)
        message(${OGRE_INCLUDE_DIR})
        message(${OGRE_LIB_DIR})


        SET(OGRE_INCLUDE_DIR ${OGRE_INCLUDE_DIR} CACHE PATH "")
        SET(OGRE_LIBRARIES ${OGRE_LIBRARIES} CACHE STRING "")
        SET(OGRE_LIB_DIR ${OGRE_LIB_DIR} CACHE PATH "")

            SET(OGRE_FOUND TRUE)

        IF (OGRE_FOUND)
                MESSAGE(STATUS "  libraries : ${OGRE_LIBRARIES} from ${OGRE_LIB_DIR}")
                MESSAGE(STATUS "  includes  : ${OGRE_INCLUDE_DIR}")
        ELSE (OGRE_FOUND)
            IF (OGRE_FIND_REQUIRED)
                MESSAGE(FATAL_ERROR "Could not find OGRE")
            ENDIF (OGRE_FIND_REQUIRED)
        ENDIF (OGRE_FOUND)
        CMAKE_POLICY(POP)
    endif()

endif()
