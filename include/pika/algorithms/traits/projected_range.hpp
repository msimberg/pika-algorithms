//  Copyright (c) 2007-2015 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>
#include <pika/functional/invoke_result.hpp>
#include <pika/iterator_support/traits/is_range.hpp>

#include <pika/algorithms/traits/projected.hpp>

#include <iterator>
#include <type_traits>

namespace pika::parallel::detail {
    ///////////////////////////////////////////////////////////////////////////
    template <typename F, typename Rng, typename Enable = void>
    struct projected_range_result_of
    {
    };

    template <typename Proj, typename Rng>
    struct projected_range_result_of<Proj, Rng,
        std::enable_if_t<pika::traits::is_range<Rng>::value>>
      : projected_result_of<std::decay_t<Proj>,
            typename pika::traits::range_iterator<Rng>::type>
    {
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Proj, typename Rng, typename Enable = void>
    struct is_projected_range : std::false_type
    {
    };

    template <typename Proj, typename Rng>
    struct is_projected_range<Proj, Rng,
        std::enable_if_t<pika::traits::is_range<Rng>::value>>
      : is_projected<std::decay_t<Proj>,
            typename pika::traits::range_iterator<Rng>::type>
    {
    };

    template <typename Proj, typename Rng>
    inline constexpr bool is_projected_range_v =
        is_projected_range<Proj, Rng>::value;

    ///////////////////////////////////////////////////////////////////////////
    template <typename Proj, typename Rng, typename Enable = void>
    struct projected_range
    {
    };

    template <typename Proj, typename Rng>
    struct projected_range<Proj, Rng,
        std::enable_if_t<pika::traits::is_range<Rng>::value>>
    {
        using projector_type = std::decay_t<Proj>;
        using iterator_type = typename pika::traits::range_iterator<Rng>::type;
    };
}    // namespace pika::parallel::detail
