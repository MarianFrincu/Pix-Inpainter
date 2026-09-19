cmake_minimum_required(VERSION 3.21)

project(PixInpainter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB UI_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")

# WIN32 hides the console window on Windows; it is ignored on other platforms.
qt_add_executable(PixInpainter WIN32
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    resources/PixInpainter.qrc
)

target_include_directories(PixInpainter PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(PixInpainter PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)

install(TARGETS PixInpainter RUNTIME DESTINATION bin)
