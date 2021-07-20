

add_library(boost_typeof INTERFACE)
add_library(Boost::typeof ALIAS boost_typeof)

target_include_directories(boost_typeof INTERFACE include)

target_link_libraries(boost_typeof
    INTERFACE
        Boost::config
        Boost::preprocessor
        Boost::type_traits
)

