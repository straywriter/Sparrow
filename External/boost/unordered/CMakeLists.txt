



add_library(boost_unordered INTERFACE)
add_library(Boost::unordered ALIAS boost_unordered)

target_include_directories(boost_unordered INTERFACE include)

target_link_libraries(boost_unordered
  INTERFACE
    Boost::assert
    Boost::config
    Boost::container
    Boost::container_hash
    Boost::core
    Boost::detail
    Boost::move
    Boost::predef
    Boost::preprocessor
    Boost::smart_ptr
    Boost::throw_exception
    Boost::tuple
    Boost::type_traits
)
