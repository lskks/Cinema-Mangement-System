cmake_minimum_required(VERSION 4.0)

project(TTMS C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(Common)
add_subdirectory(Persistence)
add_subdirectory(Service)
add_subdirectory(View)

add_executable(${PROJECT_NAME} TTMS.c)

target_link_libraries(${PROJECT_NAME}
    CommonLib
    PersistenceLib
    ServiceLib
    ViewLib
)