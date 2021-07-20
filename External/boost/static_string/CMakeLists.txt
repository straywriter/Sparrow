
add_library(boost_static_string INTERFACE)
add_library(Boost::static_string ALIAS boost_static_string)

target_include_directories(boost_static_string INTERFACE include)


