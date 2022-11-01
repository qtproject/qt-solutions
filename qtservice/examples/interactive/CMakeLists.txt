# Author: Kang Lin <kl222@126.com>

project(interactive)

SET(QT_COMPONENTS Gui Widgets)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        LIST(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
endif()
message("${PROJECT_NAME} QT_LIBRARIES:${QT_LIBRARIES}")

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE QtService ${QT_LIBRARIES})
