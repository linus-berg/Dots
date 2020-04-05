set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS system REQUIRED)
find_package(libtar)

include_directories(${PROJECT_SOURCE_DIR}/include)
IF(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
ENDIF(Boost_FOUND)
if (libtar_FOUND)
  include_directories(${libtar_INCLUDE_DIRS})a
ENDIF(libtar_FOUND)

# Libs
# add_subdirectory(example)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${libtar_LIBRARIES} pthread)
