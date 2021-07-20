
add_library( boost_predef INTERFACE )

target_include_directories( boost_predef INTERFACE include )

add_library( Boost::predef ALIAS boost_predef )
