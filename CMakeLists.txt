project(VirtualBot)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(VirtualBot_SRCS VirtualBot.cpp main.cpp)
qt4_automoc(${VirtualBot_SRCS})
add_executable(VirtualBot ${VirtualBot_SRCS})
target_link_libraries(VirtualBot ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
