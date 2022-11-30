//  Copyright (c) 2019-2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>
#include <pika/functional/invoke.hpp>

#include <pika/parallel/algorithms/detail/distance.hpp>
#include <pika/parallel/algorithms/detail/predicates.hpp>

#include <algorithm>
#include <functional>
#include <iterator>

namespace pika::parallel::detail {

    // provide implementation of std::rotate supporting iterators/sentinels
    template <typename Iter, typename Sent>
    inline constexpr void
    sequential_rotate_helper(Iter first, Iter new_first, Sent last)
    {
        Iter next = new_first;
        while (first != next)
        {
#if defined(PIKA_ALGORITHMS_HAVE_CXX20_STD_RANGES_ITER_SWAP)
            std::ranges::iter_swap(first++, next++);
#else
            std::iter_swap(first++, next++);
#endif
            if (next == last)
            {
                next = new_first;
            }
            else if (first == new_first)
            {
                new_first = next;
            }
        }
    }

    template <typename Iter, typename Sent>
    inline constexpr in_out_result<Iter, Sent>
    sequential_rotate(Iter first, Iter new_first, Sent last)
    {
        if (first != new_first && new_first != last)
            sequential_rotate_helper(first, new_first, last);

        detail::advance(first, detail::distance(new_first, last));
        return {first, last};
    }
}    // namespace pika::parallel::detail
