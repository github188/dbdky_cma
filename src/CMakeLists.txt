add_subdirectory(utils)
add_subdirectory(port)
add_subdirectory(dbhelper)
add_subdirectory(ws)

set(confUtil_SRCS
    ConfigUtil.cpp
)

include_directories(${THIRD_PARTY_DIR}/tinyxml)
add_library(confutil ${confUtil_SRCS})
target_link_libraries(confutil tinyxml utils)
install(TARGETS confutil DESTINATION lib)

set(cma_server_SRCS
  cma_server.cpp
  cma_frame.cpp
  cma_ptlrender.cpp
  cma_ptl_fbjc.cpp
  main.cpp
)

add_executable(cma_server ${cma_server_SRCS})
target_link_libraries(cma_server confutil port utils dbhelper)
install(TARGETS cma_server DESTINATION bin)

set(cma_server_legacy_SRCS
  cma_server_legacy.cpp
  cma_server_legacy_main.cpp
)

add_executable(cma_server_legacy ${cma_server_legacy_SRCS})
target_link_libraries(cma_server_legacy port utils dbhelper)
install(TARGETS cma_server_legacy DESTINATION bin)

set(cma_uploadservice_SRCS
  cma_uploadservice.cpp
  cma_uploadservice_main.cpp
)

add_executable(cma_uploadservice ${cma_uploadservice_SRCS})
include_directories(.)
target_link_libraries(cma_uploadservice confutil port utils dbhelper tinyxml ws_interface)
install(TARGETS cma_uploadservice DESTINATION bin)
