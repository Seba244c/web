# Root Project Cmake
# Includes Library Kraken
# Includes Application Sandbox
cmake_minimum_required (VERSION 3.28)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# No minmax
add_definitions(-DNOMINMAX)

# Project
project ("web")
add_executable(web src/Web.cpp)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT web)

# Depedencies
add_subdirectory(vendor/tomlplusplus)
target_link_libraries(web PRIVATE tomlplusplus::tomlplusplus) 

# install
install(TARGETS web DESTINATION /usr/local/bin/)
