set(syslog_SRCS
  syslog.cpp
)

add_library(syslog ${syslog_SRCS})
target_link_libraries(syslog redisclient)
install(TARGETS syslog DESTINATION lib)