set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system filesystem REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(arc)
add_subdirectory(crypto)
add_subdirectory(filemgr)

# Libs
# add_subdirectory(example)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(
  ${PROJECT_NAME}
  ${Boost_LIBRARIES}
  ${OpenSSL_LIBRARIES}
  arc
  pthread
)
