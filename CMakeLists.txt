cmake_minimum_required(VERSION 3.14)
project(GlobalSingleton)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SRC SingletonManager/*.cpp)
add_library(sgtmgr SHARED ${SRC})

install(TARGETS sgtmgr
  ARCHIVE DESTINATION library # .lib/.a
  LIBRARY DESTINATION runtime # .dll/.so
  RUNTIME DESTINATION runtime # .exe/.out
)
install(DIRECTORY Common/ DESTINATION include)
install(FILES LICENSE.txt DESTINATION .)
install(FILES README.txt DESTINATION .)
