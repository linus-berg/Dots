set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(LibArchive REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${PROJECT_SOURCE_DIRS}/include)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${LibArchive_INCLUDE_DIRS})

# Libs
# add_subdirectory(example)
add_library(arc main.cpp)
target_link_libraries(
  arc
  ${Boost_LIBRARIES}
  ${LibArchive_LIBRARIES}
  enc
  pthread
)
