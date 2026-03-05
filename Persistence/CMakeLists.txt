# 设置 Persistence 模块
add_library(PersistenceLib
    EntityKey_Persist.c
    Seat_Persist.c
    Studio_Persist.c
)

# 指定头文件的搜索目录
target_include_directories(PersistenceLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 声明对 CommonLib 的依赖
target_link_libraries(PersistenceLib CommonLib)