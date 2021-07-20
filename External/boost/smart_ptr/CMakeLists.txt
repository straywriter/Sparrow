

add_library(boost_smart_ptr INTERFACE)
add_library(Boost::smart_ptr ALIAS boost_smart_ptr)

target_include_directories(boost_smart_ptr INTERFACE include)

target_link_libraries(boost_smart_ptr
  INTERFACE
    Boost::assert
    Boost::config
    Boost::core
    Boost::move
    Boost::static_assert
    Boost::throw_exception
    Boost::type_traits
)
