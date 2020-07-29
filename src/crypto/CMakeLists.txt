find_package(OpenSSL REQUIRED)

include_directories(${PROJECT_SOURCE_DIRS}/include)
include_directories(${OpenSSL_INCLUDE_DIRS})

add_library(enc main.cpp)
target_link_libraries(
  enc
  ${OpenSSL_LIBRARIES}
  ssl
  crypto
)
