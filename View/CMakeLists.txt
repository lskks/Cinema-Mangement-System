add_library(ViewLib
    Main_Menu.c
    Seat_UI.c
    Studio_UI.c
    Play.c
    Account_UI.c
    MaiAccount_UI.c
    Sale_UI.c
)

target_include_directories(ViewLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ViewLib ServiceLib)