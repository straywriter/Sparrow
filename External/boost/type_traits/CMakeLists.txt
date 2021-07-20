


add_library(boost_type_traits INTERFACE)
add_library(Boost::type_traits ALIAS boost_type_traits)

target_include_directories(boost_type_traits INTERFACE include)

target_link_libraries(boost_type_traits
  INTERFACE
    Boost::config
    Boost::static_assert
)

