add_library(boost_static_assert INTERFACE)
add_library(Boost::static_assert ALIAS boost_static_assert)

target_include_directories(boost_static_assert INTERFACE include)

target_link_libraries(boost_static_assert
  INTERFACE
    Boost::config
)


