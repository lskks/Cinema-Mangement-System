add_library(CommonLib
        common.c
        listTestDrv.c
)

target_include_directories(CommonLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})