# Copyright 2019 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt



add_library(boost_throw_exception INTERFACE)
add_library(Boost::throw_exception ALIAS boost_throw_exception)

target_include_directories(boost_throw_exception INTERFACE include)

target_link_libraries(boost_throw_exception
  INTERFACE
    Boost::assert
    Boost::config
)
