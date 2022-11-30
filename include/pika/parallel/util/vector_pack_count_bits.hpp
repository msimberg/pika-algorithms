//  Copyright (c) 2016-2017 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>

#include <cstddef>

///////////////////////////////////////////////////////////////////////////////
namespace pika::parallel::traits::detail {
    PIKA_HOST_DEVICE PIKA_FORCEINLINE std::size_t count_bits(bool value)
    {
        return value ? 1 : 0;
    }
}    // namespace pika::parallel::traits::detail

#if defined(PIKA_HAVE_DATAPAR)

#if !defined(__CUDACC__)
#include <pika/parallel/util/detail/simd/vector_pack_count_bits.hpp>
#endif

#endif
