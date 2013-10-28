set(ws_interface_SRCS
  test/soapCAGAccessServiceHttpBindingProxy.cpp
  test/soapC.cpp
  test/soapCAGAccessServiceHttpBindingService.cpp
)

add_library(ws_interface ${ws_interface_SRCS})
target_link_libraries(ws_interface gsoap++)
install(TARGETS ws_interface DESTINATION lib)
