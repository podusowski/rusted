add_executable(
    server
    Network/Connection.cpp
    Network/ConnectionContext.cpp
    Network/ServerController.cpp
    Services/ServerInfoService.cpp
    Services/AuthorizationService.cpp
    Services/EntityService.cpp
    Services/PlayerService.cpp
    Services/RustedTimeService.cpp
    Services/AdministrationService.cpp
    Services/ServiceDeployment.cpp
    Services/Utils.cpp
    Game/UniverseDataBaseFacade.cpp
    Game/PlayerContainer.cpp
    Game/ShipClass.cpp
    Game/ShipClassContainer.cpp
    Game/Actions/Attack.cpp
    Game/Actions/BuildShip.cpp
    Game/Actions/Gather.cpp
    Game/Actions/Transfer.cpp
    Game/Actions/ActionFactory.cpp
    Game/Actions/ActionPerformer.cpp
    Game/ObjectVisibilityUpdater.cpp
    DataBase/SociSessionFactory.cpp
    main.cpp
)

target_link_libraries(server PRIVATE common Cake pthread soci_core hashlibpp)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
