add_subdirectory(utils)
add_subdirectory(port)
add_subdirectory(dbhelper)
add_subdirectory(ws)
add_subdirectory(syslog)

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
  cma_ptl_dxwd.cpp
  cma_ptl_gt.cpp
  cma_ptl_qx.cpp
  cma_ptl_zdtz.cpp
  cma_ptl_hc.cpp
  cma_ptl_fp.cpp
  cma_ptl_wh.cpp
  cma_ptl_wdtz.cpp
  main.cpp
)

add_executable(cma_server ${cma_server_SRCS})
target_link_libraries(cma_server confutil port utils dbhelper)
install(TARGETS cma_server DESTINATION bin)

set(cma_server_legacy_SRCS
  cma_server_legacy.cpp
  cma_ptlrender_legacy.cpp
  cma_ptl_legacy_qx.cpp
  cma_ptl_legacy_dxwd.cpp
  cma_ptl_legacy_fbjc_cz.cpp
  cma_ptl_legacy_fbjc_jd.cpp
  cma_frame_legacy.cpp
  #cma_session_state.cpp
  cma_session_wait.cpp
  cma_session_hasdata.cpp
  cma_session_imageinit.cpp
  cma_session_imagerecvreq.cpp
  cma_session_imagetrans.cpp
  cma_session_imagetransfinish.cpp
  cma_session.cpp
  cma_datasession.cpp
  cma_imagesession.cpp
  cma_sessionpool.cpp
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
