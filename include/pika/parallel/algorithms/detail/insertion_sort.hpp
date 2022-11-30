//  Copyright (c) 2015-2017 Francisco Jose Tapia
//  Copyright (c) 2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>

#include <iterator>
#include <utility>

namespace pika::parallel::detail {
    /// @brief : Insertion sort algorithm
    /// @param [in] first: iterator to the first element of the range
    /// @param [in] last : iterator to the next element of the last in the range
    /// @param [in] comp : object for to do the comparison between the elements
    /// @remarks This algorithm is O(N^2)
    template <typename Iter,
        typename Compare =
            std::less<typename std::iterator_traits<Iter>::value_type>>
    void insertion_sort(Iter first, Iter last, Compare comp = Compare())
    {
        using value_type = typename std::iterator_traits<Iter>::value_type;

        if (last - first < 2)
        {
            return;
        }

        for (Iter alfa = first + 1; alfa != last; ++alfa)
        {
            value_type aux = PIKA_MOVE(*alfa);
            Iter beta = alfa;

            while (beta != first && comp(aux, *(beta - 1)))
            {
                *beta = PIKA_MOVE(*(beta - 1));
                --beta;
            }

            *beta = PIKA_MOVE(aux);
        }
    }
}    // namespace pika::parallel::detail
