add_library(PersistenceLib
    EntityKey_Persist.c
    Seat_Persist.c
    Studio_Persist.c
        Ticket.c
        Ticket.h
)

target_include_directories(PersistenceLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(PersistenceLib CommonLib)