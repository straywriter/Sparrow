


add_library(boost_config INTERFACE)
add_library(Boost::config ALIAS boost_config)

target_include_directories(boost_config INTERFACE include)

