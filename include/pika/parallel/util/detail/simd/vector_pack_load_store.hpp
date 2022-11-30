//  Copyright (c) 2021 Srinivas Yadav
//  Copyright (c) 2016-2017 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>

#if defined(PIKA_ALGORITHMS_HAVE_STD_EXPERIMENTAL_SIMD)
#include <cstddef>
#include <iterator>
#include <memory>

#include <experimental/simd>

namespace pika::parallel::traits::detail {
    template <typename V, typename ValueType, typename Enable>
    struct vector_pack_load
    {
        template <typename Iter>
        static V aligned(Iter const& iter)
        {
            return V(std::addressof(*iter), std::experimental::vector_aligned);
        }

        template <typename Iter>
        static V unaligned(Iter const& iter)
        {
            return V(std::addressof(*iter), std::experimental::element_aligned);
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename V, typename ValueType, typename Enable>
    struct vector_pack_store
    {
        template <typename Iter>
        static void aligned(V& value, Iter const& iter)
        {
            value.copy_to(
                std::addressof(*iter), std::experimental::vector_aligned);
        }

        template <typename Iter>
        static void unaligned(V& value, Iter const& iter)
        {
            value.copy_to(
                std::addressof(*iter), std::experimental::element_aligned);
        }
    };
}    // namespace pika::parallel::traits::detail

#endif
