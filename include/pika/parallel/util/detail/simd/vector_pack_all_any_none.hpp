//  Copyright (c) 2021 Srinivas Yadav
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>

#if defined(PIKA_ALGORITHMS_HAVE_STD_EXPERIMENTAL_SIMD)
#include <cstddef>

#include <experimental/simd>

namespace pika::parallel::traits::detail {
    template <typename T, typename Abi>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE std::size_t
    all_of(std::experimental::simd_mask<T, Abi> const& msk)
    {
        return std::experimental::all_of(msk);
    }

    ///////////////////////////////////////////////////////////////////////
    template <typename T, typename Abi>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE std::size_t
    any_of(std::experimental::simd_mask<T, Abi> const& msk)
    {
        return std::experimental::any_of(msk);
    }

    ///////////////////////////////////////////////////////////////////////
    template <typename T, typename Abi>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE std::size_t
    none_of(std::experimental::simd_mask<T, Abi> const& msk)
    {
        return std::experimental::none_of(msk);
    }
}    // namespace pika::parallel::traits::detail

#endif
