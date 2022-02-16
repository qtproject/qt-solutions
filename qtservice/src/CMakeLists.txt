# Author: Kang Lin <kl222@126.com>

cmake_minimum_required(VERSION 3.5)

if(CMAKE_VERSION VERSION_LESS "3.16.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

project(QtService)

# Open qt complie tools
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

set(QT_COMPONENTS Core)
# Need qt components
if(UNIX)
    list(APPEND QT_COMPONENTS Network)
endif()
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        LIST(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
endif()
message("${PROJECT_NAME} QT_LIBRARIES:${QT_LIBRARIES}")

set(SOURCE_FILES
    qtservice.cpp
    )

set(INSTALL_HEADER_FILES
    QtService
    QtServiceBase
    QtServiceController
    qtservice.h
    )

set(HEADER_FILES ${INSTALL_HEADER_FILES}
    qtservice_p.h
    )

if(UNIX)
    list(APPEND SOURCE_FILES
        qtservice_unix.cpp
        qtunixsocket.cpp
        qtunixserversocket.cpp)
    list(APPEND HEADER_FILES
        qtunixserversocket.h
        qtunixsocket.h)
endif()
if(WIN32)
    list(APPEND SOURCE_FILES
        qtservice_win.cpp)
endif()

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    PUBLIC_HEADER "${INSTALL_HEADER_FILES}" # Install head files
    )
target_link_libraries(${PROJECT_NAME} PUBLIC ${QT_LIBRARIES})
target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

if(WIN32)
    if(BUILD_SHARED_LIBS)
        target_compile_definitions(${PROJECT_NAME} PRIVATE QT_QTSERVICE_EXPORT UNICODE)
    endif()
    INSTALL(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
else()
    INSTALL(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
endif()
# Install cmake configure files
install(EXPORT ${PROJECT_NAME}Targets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )

export(TARGETS ${PROJECT_NAME}
       APPEND FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
       )

include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/QtServiceConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/QtServiceConfig.cmake
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )

# Install cmake version configure file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${BUILD_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/QtServiceConfig.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

configure_file(${CMAKE_SOURCE_DIR}/cmake/QtService.pc.in
    ${CMAKE_BINARY_DIR}/QtService.pc
    @ONLY)
install(FILES
    ${CMAKE_BINARY_DIR}/QtService.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
