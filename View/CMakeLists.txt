add_library(ViewLib
    Main_Menu.c
    Seat_UI.c
    Studio_UI.c
        Ticket.h
        Ticket.c
)

target_include_directories(ViewLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ViewLib ServiceLib)