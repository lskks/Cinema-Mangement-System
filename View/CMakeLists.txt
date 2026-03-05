# 设置 View 模块
add_library(ViewLib
    Main_Menu.c
    Seat_UI.c
    Studio_UI.c
)

# 指定头文件的搜索路径
target_include_directories(ViewLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 声明对 ServiceLib 的依赖
target_link_libraries(ViewLib ServiceLib)