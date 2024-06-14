
cmake_minimum_required (VERSION 3.28)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (MSVC)
  target_compile_options(core PRIVATE /std:c++latest /experimental:module)
  target_compile_options(tests PRIVATE /std:c++latest /experimental:module)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(core PRIVATE -std=c++23 -fmodules-ts)
  target_compile_options(tests PRIVATE -std=c++23 -fmodules-ts)
endif()

project ("Format")

add_subdirectory ("Format")
