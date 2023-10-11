cmake_minimum_required(VERSION 3.27)
#-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
#include(C:/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKG_DIR "C:/vcpkg/installed/x64-windows")
set(VCPKG_TARGET_TRIPLET "x64-windows"
      CACHE STRING "")
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")

set(SRCROOT src)
set(INCROOT include)

project(Sistema_de_banco)

set(SRC
      ${SRCROOT}/main.cpp
      ${INCROOT}/cliente.cpp
      ${INCROOT}/cliente.hpp
)

include_directories("C:/vcpkg/installed/x64-windows/include")
link_directories("C:/vcpkg/installed/x64-windows/lib")

add_executable(Sistema_de_banco ${SRC})

target_link_libraries(Sistema_de_banco PRIVATE sqlite3)
target_include_directories(Sistema_de_banco PRIVATE ${INCROOT})

