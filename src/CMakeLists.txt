add_subdirectory(utils)
add_subdirectory(port)
add_subdirectory(dbhelper)

set(confUtil_SRCS
    ConfigUtil.cpp
)

include_directories(${THIRD_PARTY_DIR}/tinyxml)
add_library(confutil ${confUtil_SRCS})
target_link_libraries(confutil tinyxml utils)
install(TARGETS confutil DESTINATION lib)

set(cma_server_SRCS
  cma_server.cpp
  main.cpp
)

add_executable(cma_server ${cma_server_SRCS})
target_link_libraries(cma_server confutil port utils)
install(TARGETS cma_server DESTINATION bin)
