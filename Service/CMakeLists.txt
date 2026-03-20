add_library(ServiceLib
    Seat.c
    Studio.c
    Account.c
    Play.c
    Schedule.c
)

target_include_directories(ServiceLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ServiceLib PersistenceLib)
