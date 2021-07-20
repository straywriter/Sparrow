add_library (boost_intrusive INTERFACE )
add_library( Boost::intrusive ALIAS boost_intrusive )

target_include_directories( boost_intrusive INTERFACE include )

target_link_libraries( boost_intrusive
    INTERFACE
        Boost::assert
        Boost::config
        Boost::container_hash
        Boost::core
        Boost::move
        Boost::static_assert
)
