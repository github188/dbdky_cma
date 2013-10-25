set(fbjc_unittest_SRCS
  fbjc_unittest.cpp
  cma_client.cpp
)

add_executable(fbjc_unittest ${fbjc_unittest_SRCS})
target_link_libraries(fbjc_unittest port utils)
install(TARGETS fbjc_unittest DESTINATION bin)


set(qxjc_unittest_SRCS
  qxjc_unittest.cpp
  cma_client.cpp
)

add_executable(qxjc_unittest ${qxjc_unittest_SRCS})
target_link_libraries(qxjc_unittest port utils)
install(TARGETS qxjc_unittest DESTINATION bin)

set(dxhcjc_unittest_SRCS
  dxhcjc_unittest.cpp
  cma_client.cpp
)

add_executable(dxhcjc_unittest ${dxhcjc_unittest_SRCS})
target_link_libraries(dxhcjc_unittest port utils)
install(TARGETS dxhcjc_unittest DESTINATION bin)


set(gtjc_unittest_SRCS
  gtjc_unittest.cpp
  cma_client.cpp
)

add_executable(gtjc_unittest ${gtjc_unittest_SRCS})
target_link_libraries(gtjc_unittest port utils)
install(TARGETS gtjc_unittest DESTINATION bin)


set(wdjc_unittest_SRCS
  wdjc_unittest.cpp
  cma_client.cpp
)

add_executable(wdjc_unittest ${wdjc_unittest_SRCS})
target_link_libraries(wdjc_unittest port utils)
install(TARGETS wdjc_unittest DESTINATION bin)
