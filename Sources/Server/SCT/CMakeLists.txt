
add_executable(server_sct
    mainSCT.cpp
    Core/Component.cpp
    Core/Connection.cpp
    Core/Configuration.cpp

    ServerVersionSct.cpp
    UserSct.cpp
    ShipSct.cpp
    ObjectsSct.cpp
    ActionsSct.cpp
    SmokeSct.cpp
)
target_link_libraries(server_sct PRIVATE  common)
target_link_libraries(server_sct PRIVATE gtest Cake soci_core)

# append $dependencies gtest
# append $dependencies SctDataBase
# append $dependencies Server
#
# append $libraries Cake
# append $libraries Common
# append $libraries gtest
# append $libraries pthread
# append $libraries soci_core
#
# target application ServerSCT sources($sources) link_with($libraries) include_dirs($include_dirs) depends_on($dependencies)
#
# # database for the SCT
#
# append $sct_database_prerequisites "${__path}/../DataBaseSchema.sql"
# append $sct_database_prerequisites "${__path}/SctDataBase.sql"
#
# set $sct_database "${__build}/SctDataBase.sqlite3"
#
# target phony SctDataBase artefacts($sct_database) \
# prerequisites($sct_database_prerequisites) \
# run_before("rm -f ${sct_database}; cat ${sct_database_prerequisites} | sqlite3 ${sct_database}")
#
#
set(DATABASE_SCHEMA ${CMAKE_CURRENT_SOURCE_DIR}/../DataBaseSchema.sql)
set(SCT_DATABASE ${CMAKE_CURRENT_BINARY_DIR}/SctDataBase.sqlite3 PARENT_SCOPE)

# Beware: dependency tracking does not work here, input file modification does not cause output file rebuild automatically
add_custom_command(
    TARGET server_sct
    COMMAND rm -f ${SCT_DATABASE}
    COMMAND cat ${DATABASE_SCHEMA} ${CMAKE_CURRENT_SOURCE_DIR}/SctDataBase.sql | sqlite3 ${SCT_DATABASE}
)

set(SCT_BIG_DATABASE_SQL ${CMAKE_CURRENT_BINARY_DIR}/BigDataBase.sql)
set(SCT_BIG_DATABASE ${CMAKE_CURRENT_BINARY_DIR}/BigDataBase.sqlite3 PARENT_SCOPE)
add_custom_command(
    TARGET server_sct
    COMMAND rm -f ${SCT_BIG_DATABASE} ${SCT_BIG_DATABASE_SQL}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../genBigDataBase.py --output ${SCT_BIG_DATABASE_SQL}
    COMMAND cat ${DATABASE_SCHEMA} ${SCT_BIG_DATABASE_SQL} | sqlite3  ${SCT_BIG_DATABASE}
)
