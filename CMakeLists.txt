cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(TinyEmbed VERSION 0.1.0 LANGUAGES C)

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX /usr/ CACHE PATH "Default install prefix" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_C_STANDARD 11)

file(GLOB te_SRC tiny-embed/*.c)
add_executable(tiny-embed ${te_SRC})

install(
  TARGETS tiny-embed
  RUNTIME DESTINATION bin
)