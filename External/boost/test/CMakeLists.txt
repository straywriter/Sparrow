file(GLOB BOOST_TEST_SRC ./src/*.cpp)

add_library(boost_test ${BOOST_TEST_SRC})
add_library(Boost::test ALIAS boost_test)

target_include_directories(boost_test PUBLIC include)

  # target_compile_options(
  #     boost_test
  #     PUBLIC $<$<CONFIG:>:/MT>
  #            $<$<CONFIG:Debug>:/MTd>
  #            $<$<CONFIG:Release>:/MT>
  #            $<$<CXX_COMPILER_ID:MSVC>:
  #           # /d1reportClassLayoutChanges
  #           # /permissive-
  #           #  /std:c++17
  #            >)

target_link_libraries(
  boost_test
  PUBLIC Boost::array
         Boost::assert
         Boost::bind
         Boost::concept_check
         Boost::config
         Boost::core
         Boost::exception
         Boost::function
         Boost::iterator
         Boost::mpl
         Boost::range
         Boost::regex
         Boost::static_assert
         Boost::throw_exception
         Boost::tuple
         Boost::type_traits
         Boost::unordered
         )
