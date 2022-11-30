//  Copyright (c) 2020 Hartmut Kaiser
//  Copyright (c) 2021 Giannis Gonidelis
//  Copyright (c) 2021 Srinivas Yadav
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>
#include <pika/execution/traits/is_execution_policy.hpp>
#include <pika/functional/detail/tag_fallback_invoke.hpp>
#include <pika/functional/invoke.hpp>
#include <pika/parallel/algorithms/detail/advance_to_sentinel.hpp>
#include <pika/parallel/util/loop.hpp>
#include <pika/parallel/util/projection_identity.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace pika::parallel::detail {
    // provide implementation of std::find supporting iterators/sentinels
    template <typename ExPolicy>
    struct sequential_find_t
      : pika::functional::detail::tag_fallback<sequential_find_t<ExPolicy>>
    {
    private:
        template <typename Iterator, typename Sentinel, typename T,
            typename Proj = projection_identity>
        friend inline constexpr Iterator
        tag_fallback_invoke(sequential_find_t<ExPolicy>, Iterator first,
            Sentinel last, T const& value, Proj proj = Proj())
        {
            for (; first != last; ++first)
            {
                if (PIKA_INVOKE(proj, *first) == value)
                {
                    return first;
                }
            }
            return first;
        }

        template <typename FwdIter, typename Token, typename T, typename Proj>
        friend inline constexpr void
        tag_fallback_invoke(sequential_find_t<ExPolicy>, std::size_t base_idx,
            FwdIter part_begin, std::size_t part_count, Token& tok,
            T const& val, Proj&& proj)
        {
            loop_idx_n<ExPolicy>(base_idx, part_begin, part_count, tok,
                [&val, &proj, &tok](auto& v, std::size_t i) -> void {
                    if (PIKA_INVOKE(proj, v) == val)
                    {
                        tok.cancel(i);
                    }
                });
        }
    };

#if !defined(PIKA_COMPUTE_DEVICE_CODE)
    template <typename ExPolicy>
    inline constexpr sequential_find_t<ExPolicy> sequential_find =
        sequential_find_t<ExPolicy>{};
#else
    template <typename ExPolicy, typename Iterator, typename Sentinel,
        typename T, typename Proj = projection_identity>
    inline constexpr Iterator sequential_find(
        Iterator first, Sentinel last, T const& val, Proj proj = Proj())
    {
        return sequential_find_t<ExPolicy>{}(first, last, val, proj);
    }

    template <typename ExPolicy, typename FwdIter, typename Token, typename T,
        typename Proj>
    inline constexpr void
    sequential_find(std::size_t base_idx, FwdIter part_begin,
        std::size_t part_count, Token& tok, T const& val, Proj&& proj)
    {
        return sequential_find_t<ExPolicy>{}(base_idx, part_begin, part_count,
            tok, val, PIKA_FORWARD(Proj, proj));
    }
#endif

    // provide implementation of std::find_if supporting iterators/sentinels
    template <typename ExPolicy>
    struct sequential_find_if_t
      : pika::functional::detail::tag_fallback<sequential_find_if_t<ExPolicy>>
    {
    private:
        template <typename Iterator, typename Sentinel, typename Pred,
            typename Proj = projection_identity>
        friend inline constexpr Iterator
        tag_fallback_invoke(sequential_find_if_t<ExPolicy>, Iterator first,
            Sentinel last, Pred pred, Proj proj = Proj())
        {
            for (; first != last; ++first)
            {
                if (PIKA_INVOKE(pred, PIKA_INVOKE(proj, *first)))
                {
                    return first;
                }
            }
            return first;
        }

        template <typename FwdIter, typename Token, typename F, typename Proj>
        friend inline constexpr void
        tag_fallback_invoke(sequential_find_if_t<ExPolicy>, FwdIter part_begin,
            std::size_t part_count, Token& tok, F&& op, Proj&& proj)
        {
            loop_n<std::decay_t<ExPolicy>>(part_begin, part_count, tok,
                [&op, &tok, &proj](auto const& curr) {
                    if (PIKA_INVOKE(op, PIKA_INVOKE(proj, *curr)))
                    {
                        tok.cancel();
                    }
                });
        }

        template <typename FwdIter, typename Token, typename F, typename Proj>
        friend inline constexpr void
        tag_fallback_invoke(sequential_find_if_t<ExPolicy>,
            std::size_t base_idx, FwdIter part_begin, std::size_t part_count,
            Token& tok, F&& f, Proj&& proj)
        {
            loop_idx_n<ExPolicy>(base_idx, part_begin, part_count, tok,
                [&f, &proj, &tok](auto& v, std::size_t i) -> void {
                    if (PIKA_INVOKE(f, PIKA_INVOKE(proj, v)))
                    {
                        tok.cancel(i);
                    }
                });
        }
    };

#if !defined(PIKA_COMPUTE_DEVICE_CODE)
    template <typename ExPolicy>
    inline constexpr sequential_find_if_t<ExPolicy> sequential_find_if =
        sequential_find_if_t<ExPolicy>{};
#else
    template <typename ExPolicy, typename Iterator, typename Sentinel,
        typename Pred, typename Proj = projection_identity>
    inline constexpr Iterator sequential_find_if(
        Iterator first, Sentinel last, Pred pred, Proj proj = Proj())
    {
        return sequential_find_if_t<ExPolicy>{}(first, last, pred, proj);
    }

    template <typename ExPolicy, typename FwdIter, typename Token, typename F,
        typename Proj>
    inline constexpr void sequential_find_if(FwdIter part_begin,
        std::size_t part_count, Token& tok, F&& op, Proj&& proj)
    {
        return sequential_find_if_t<ExPolicy>{}(part_begin, part_count, tok,
            PIKA_FORWARD(F, op), PIKA_FORWARD(Proj, proj));
    }

    template <typename ExPolicy, typename FwdIter, typename Token, typename F,
        typename Proj>
    inline constexpr void
    sequential_find_if(std::size_t base_idx, FwdIter part_begin,
        std::size_t part_count, Token& tok, F&& f, Proj&& proj)
    {
        return sequential_find_if_t<ExPolicy>{}(base_idx, part_begin,
            part_count, tok, PIKA_FORWARD(F, f), PIKA_FORWARD(Proj, proj));
    }
#endif

    // provide implementation of std::find_if_not supporting iterators/sentinels
    template <typename ExPolicy>
    struct sequential_find_if_not_t
      : pika::functional::detail::tag_fallback<
            sequential_find_if_not_t<ExPolicy>>
    {
    private:
        template <typename Iterator, typename Sentinel, typename Pred,
            typename Proj = projection_identity>
        friend inline constexpr Iterator
        tag_fallback_invoke(sequential_find_if_not_t<ExPolicy>, Iterator first,
            Sentinel last, Pred pred, Proj proj = Proj())
        {
            for (; first != last; ++first)
            {
                if (!PIKA_INVOKE(pred, PIKA_INVOKE(proj, *first)))
                {
                    return first;
                }
            }
            return first;
        }

        template <typename FwdIter, typename Token, typename F, typename Proj>
        friend inline constexpr void tag_fallback_invoke(
            sequential_find_if_not_t<ExPolicy>, FwdIter part_begin,
            std::size_t part_count, Token& tok, F&& op, Proj&& proj)
        {
            loop_n<std::decay_t<ExPolicy>>(part_begin, part_count, tok,
                [&op, &tok, &proj](auto const& curr) {
                    if (!PIKA_INVOKE(op, PIKA_INVOKE(proj, *curr)))
                    {
                        tok.cancel();
                    }
                });
        }

        template <typename FwdIter, typename Token, typename F, typename Proj>
        friend inline constexpr void
        tag_fallback_invoke(sequential_find_if_not_t<ExPolicy>,
            std::size_t base_idx, FwdIter part_begin, std::size_t part_count,
            Token& tok, F&& f, Proj&& proj)
        {
            loop_idx_n<ExPolicy>(base_idx, part_begin, part_count, tok,
                [&f, &proj, &tok](auto& v, std::size_t i) -> void {
                    if (!PIKA_INVOKE(f, PIKA_INVOKE(proj, v)))
                    {
                        tok.cancel(i);
                    }
                });
        }
    };

#if !defined(PIKA_COMPUTE_DEVICE_CODE)
    template <typename ExPolicy>
    inline constexpr sequential_find_if_not_t<ExPolicy> sequential_find_if_not =
        sequential_find_if_not_t<ExPolicy>{};
#else
    template <typename ExPolicy, typename Iterator, typename Sentinel,
        typename Pred, typename Proj = projection_identity>
    inline constexpr Iterator sequential_find_if_not(
        Iterator first, Sentinel last, Pred pred, Proj proj = Proj())
    {
        return sequential_find_if_not_t<ExPolicy>{}(first, last, pred, proj);
    }

    template <typename ExPolicy, typename FwdIter, typename Token, typename F,
        typename Proj>
    inline constexpr void sequential_find_if_not(FwdIter part_begin,
        std::size_t part_count, Token& tok, F&& op, Proj&& proj)
    {
        return sequential_find_if_not_t<ExPolicy>{}(part_begin, part_count, tok,
            PIKA_FORWARD(F, op), PIKA_FORWARD(Proj, proj));
    }

    template <typename ExPolicy, typename FwdIter, typename Token, typename F,
        typename Proj>
    inline constexpr void
    sequential_find_if_not(std::size_t base_idx, FwdIter part_begin,
        std::size_t part_count, Token& tok, F&& f, Proj&& proj)
    {
        return sequential_find_if_not_t<ExPolicy>{}(base_idx, part_begin,
            part_count, tok, PIKA_FORWARD(F, f), PIKA_FORWARD(Proj, proj));
    }
#endif
}    // namespace pika::parallel::detail
