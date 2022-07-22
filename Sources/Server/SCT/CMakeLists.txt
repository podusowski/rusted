
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

set(DATABASE_SCHEMA ${CMAKE_CURRENT_SOURCE_DIR}/../DataBaseSchema.sql)
set(SCT_DATABASE ${CMAKE_CURRENT_BINARY_DIR}/SctDataBase.sqlite3)

# Beware: dependency tracking does not work here, input file modification does not cause output file rebuild automatically
add_custom_command(
    TARGET server_sct
    COMMAND rm -f ${SCT_DATABASE}
    COMMAND cat ${DATABASE_SCHEMA} ${CMAKE_CURRENT_SOURCE_DIR}/SctDataBase.sql | sqlite3 ${SCT_DATABASE}
)

set(SCT_BIG_DATABASE_SQL ${CMAKE_CURRENT_BINARY_DIR}/BigDataBase.sql)
set(SCT_BIG_DATABASE ${CMAKE_CURRENT_BINARY_DIR}/BigDataBase.sqlite3)
add_custom_command(
    TARGET server_sct
    COMMAND rm -f ${SCT_BIG_DATABASE} ${SCT_BIG_DATABASE_SQL}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../genBigDataBase.py --output ${SCT_BIG_DATABASE_SQL}
    COMMAND cat ${DATABASE_SCHEMA} ${SCT_BIG_DATABASE_SQL} | sqlite3  ${SCT_BIG_DATABASE}
)

#export variables, ugly
set(SERVER_SCT_DATABASE ${SCT_DATABASE} PARENT_SCOPE)
set(SERVER_SCT_BIG_DATABASE ${SCT_BIG_DATABASE} PARENT_SCOPE)
