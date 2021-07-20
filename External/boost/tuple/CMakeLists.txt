
add_library(boost_tuple INTERFACE)
add_library(Boost::tuple ALIAS boost_tuple)

target_include_directories(boost_tuple INTERFACE include)

target_link_libraries(boost_tuple
    INTERFACE
        Boost::config
        Boost::core
        Boost::static_assert
        Boost::type_traits
)
