set(timestamp_ex_SRCS
  timestamp_ex.cpp
)

add_executable(timestamp_ex ${timestamp_ex_SRCS})
target_link_libraries(timestamp_ex port utils)
install(TARGETS timestamp_ex DESTINATION bin)

set(thread_ex_SRCS
  thread_ex.cpp
)

add_executable(thread_ex ${thread_ex_SRCS})
target_link_libraries(thread_ex port utils)
install(TARGETS thread_ex DESTINATION bin)

set(threadpool_ex_SRCS
  threadpool_ex.cpp
)

add_executable(threadpool_ex ${threadpool_ex_SRCS})
target_link_libraries(threadpool_ex port utils)
install(TARGETS threadpool_ex DESTINATION bin)

set(fileutil_ex_SRCS
  fileutil_ex.cpp
)

add_executable(fileutil_ex ${fileutil_ex_SRCS})
target_link_libraries(fileutil_ex port utils)
install(TARGETS fileutil_ex DESTINATION bin)

set(condition_ex_SRCS
  condition_ex.cpp
)

add_executable(condition_ex ${condition_ex_SRCS})
target_link_libraries(condition_ex port utils)
install(TARGETS condition_ex DESTINATION bin)

set(buffer_ex_SRCS
  buffer_ex.cpp
)

add_executable(buffer_ex ${buffer_ex_SRCS})
target_link_libraries(buffer_ex port utils)
install(TARGETS buffer_ex DESTINATION bin)

set(dbhelper_ex_SRCS
  dbhelper_ex.cpp
)

add_executable(dbhelper_ex ${dbhelper_ex_SRCS})
target_link_libraries(dbhelper_ex dbhelper port utils)
install(TARGETS dbhelper_ex DESTINATION bin)

set(eventloop_ex_SRCS
  eventloop_ex.cpp
)

add_executable(eventloop_ex ${eventloop_ex_SRCS})
target_link_libraries(eventloop_ex port utils)
install(TARGETS eventloop_ex DESTINATION bin)
