
add_library( boost_exception INTERFACE )
add_library( Boost::exception ALIAS boost_exception )

target_include_directories( boost_exception INTERFACE include )

target_link_libraries( boost_exception
    INTERFACE
        Boost::assert
        Boost::config
        Boost::core
        Boost::smart_ptr
        Boost::throw_exception
        Boost::tuple
        Boost::type_traits
)

