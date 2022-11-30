//  Copyright (c) 2019-2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>
#include <pika/functional/invoke.hpp>

#include <functional>

namespace pika::parallel::detail {
    // provide implementation of std::accumulate supporting iterators/sentinels
    template <typename Iter, typename Sent, typename T, typename F>
    inline constexpr T accumulate(Iter first, Sent last, T value, F&& reduce_op)
    {
        for (/**/; first != last; ++first)
        {
            value = PIKA_INVOKE(reduce_op, value, *first);
        }
        return value;
    }

    template <typename Iter, typename Sent, typename T>
    inline constexpr T accumulate(Iter first, Sent last, T value)
    {
        return accumulate(first, last, value, std::plus<T>());
    }
}    // namespace pika::parallel::detail
