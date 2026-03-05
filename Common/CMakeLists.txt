# 设置 Common 模块
add_library(CommonLib common.c listTestDrv.c)

# 指定头文件的搜索目录
target_include_directories(CommonLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})