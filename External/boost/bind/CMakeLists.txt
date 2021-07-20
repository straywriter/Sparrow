add_library(boost_bind INTERFACE)
add_library(Boost::bind ALIAS boost_bind)

target_include_directories(boost_bind INTERFACE include)

target_link_libraries(boost_bind
  INTERFACE
    Boost::config
    Boost::core
)


