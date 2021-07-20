

add_library(boost_range INTERFACE)
add_library(Boost::range ALIAS boost_range)

target_include_directories(boost_range INTERFACE include)

target_link_libraries(boost_range
  INTERFACE
    Boost::array
    Boost::assert
    Boost::concept_check
    Boost::config
    Boost::container_hash
    Boost::conversion
    Boost::core
    Boost::detail
    Boost::iterator
    Boost::mpl
    Boost::optional
    Boost::preprocessor
    Boost::regex
    Boost::static_assert
    Boost::tuple
    Boost::type_traits
    Boost::utility
)

# if(BUILD_TESTING AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")

#   add_subdirectory(test)

# endif