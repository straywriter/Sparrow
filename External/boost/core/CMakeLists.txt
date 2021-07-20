
add_library(boost_core INTERFACE)
add_library(Boost::core ALIAS boost_core)

target_include_directories(boost_core INTERFACE include)

target_link_libraries(boost_core
  INTERFACE
    Boost::assert
    Boost::config
)


