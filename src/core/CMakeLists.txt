add_library(core
  csvparser.cpp
  helper.cpp
  preferences.cpp
  )

target_link_libraries(core ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
