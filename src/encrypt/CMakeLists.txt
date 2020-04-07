include_directories(${PROJECT_SOURCE_DIRS}/include)

FIND_LIBRARY(libssl libssl ${CMAKE_SOURCE_DIR}/lib/libssl)
FIND_LIBRARY(libcrypto libcrypto ${CMAKE_SOURCE_DIR}/lib/libcrypto)

add_library(enc main.cpp)
target_link_libraries(enc
  ${CMAKE_SOURCE_DIR}/lib/libssl.so.3
  ${CMAKE_SOURCE_DIR}/lib/libcrypto.so.3
)
