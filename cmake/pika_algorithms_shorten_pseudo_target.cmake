# Copyright (c) 2015 Hartmut Kaiser
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(PIKA_ALGORITHMS_SHORTENPSEUDOTARGET_LOADED TRUE)

function(pika_algorithms_shorten_pseudo_target target shortened_target)
  pika_algorithms_debug(
    "pika_algorithms_shorten_pseudo_target"
    "shortening pseudo target: ${target}"
  )
  if(WIN32)
    set(args)
    foreach(arg ${target})
      # convert to a list
      string(REPLACE "." ";" elements ${arg})
      # retrieve last element of target to be used as shortened target name
      list(GET elements -1 arg)
      set(args ${args} ${arg})
    endforeach()
    set(${shortened_target}
        ${args}
        PARENT_SCOPE
    )
    pika_algorithms_debug(
      "pika_algorithms_shorten_pseudo_target"
      "shortened pseudo target: ${${shortened_target}}"
    )
  else()
    set(${shortened_target}
        ${target}
        PARENT_SCOPE
    )
  endif()
endfunction()
