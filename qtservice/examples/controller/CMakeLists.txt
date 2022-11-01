# Author: Kang Lin <kl222@126.com>

project(controller)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE QtService)
