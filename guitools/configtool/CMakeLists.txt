set(configtool_SRCS
  main.cpp
)

include_directories(${THIRD_PARTY_DIR}/tinyxml)
add_executable(configtool ${configtool_SRCS})
target_link_libraries(configtool confutil tinyxml ncurses form)
install(TARGETS configtool DESTINATION bin)
