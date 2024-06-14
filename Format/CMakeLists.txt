project("Format" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

add_library(core)
target_sources(core
  PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
      "${CMAKE_CURRENT_SOURCE_DIR}/std.ixx"
      "${CMAKE_CURRENT_SOURCE_DIR}/zm.fmt.ixx"
      "${CMAKE_CURRENT_SOURCE_DIR}/zm.wrap.ctn.ixx"
)

add_executable(tests main.cpp)
target_link_libraries(tests PRIVATE core)
