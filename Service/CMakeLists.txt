# 设置 Service 模块
add_library(ServiceLib
    Seat.c
    Studio.c
)

# 指定头文件的搜索路径
target_include_directories(ServiceLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 声明对 PersistenceLib 的依赖
target_link_libraries(ServiceLib PersistenceLib)