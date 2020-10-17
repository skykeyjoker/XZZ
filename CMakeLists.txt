cmake_minimum_required(VERSION 3.14)

project(MiraiDemo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MIRAI_CPP_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
add_subdirectory(mirai-cpp)
include_directories(include)

#find_package(Qt5 COMPONENTS Core Network REQUIRED)
find_package(Qt5 COMPONENTS Core Network)

add_executable(MiraiDemo
  main.cpp
  pkginfo.h
  pkgquery.h
  pkgquery.cpp
  mnerinfo.h
  mnerquery.h
  mnerquery.cpp
  setuinfo.h
  setuquery.h
  setuquery.cpp
  cnpkgquery.h
  cnpkgquery.cpp
  wikiquery.h
  wikiquery.cpp
  whitelist.h
  wikiresult.h
  wikiresult.cpp
)

target_link_libraries(MiraiDemo Qt5::Core Qt5::Network mirai-cpp pthread)

target_compile_definitions(${PROJECT_NAME} PRIVATE MIRAICPP_STATICLIB)

