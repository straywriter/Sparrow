
add_library(boost_algorithm INTERFACE)
add_library(Boost::algorithm ALIAS boost_algorithm)

target_include_directories(boost_algorithm INTERFACE include)

target_link_libraries(boost_algorithm
  INTERFACE
    Boost::array
    Boost::assert
    Boost::bind
    Boost::concept_check
    Boost::config
    Boost::core
    Boost::exception
    Boost::function
    Boost::iterator
    Boost::mpl
    Boost::range
    Boost::regex
    Boost::static_assert
    Boost::throw_exception
    Boost::tuple
    Boost::type_traits
    Boost::unordered
)
