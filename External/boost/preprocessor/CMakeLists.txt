
add_library(boost_preprocessor INTERFACE)
add_library(Boost::preprocessor ALIAS boost_preprocessor)
target_include_directories(boost_preprocessor INTERFACE include)
