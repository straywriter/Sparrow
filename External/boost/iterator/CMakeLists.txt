add_library(boost_iterator INTERFACE)
add_library(Boost::iterator ALIAS boost_iterator)

target_include_directories(boost_iterator INTERFACE include)

target_link_libraries(boost_iterator
    INTERFACE
        Boost::assert
        Boost::concept_check
        Boost::config
        Boost::conversion
        Boost::core
        Boost::detail
        Boost::function_types
        Boost::fusion
        Boost::mpl
        Boost::optional
        Boost::smart_ptr
        Boost::static_assert
        Boost::type_traits
        Boost::utility
)
