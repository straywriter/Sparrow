

add_library(boost_move INTERFACE)
add_library(Boost::move ALIAS boost_move)

target_include_directories(boost_move INTERFACE include)

target_link_libraries(boost_move
  INTERFACE
    Boost::assert
    Boost::config
    Boost::core
    Boost::static_assert
)

