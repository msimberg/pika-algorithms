//  Copyright (c) 2016-2021 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>
#include <pika/algorithms/traits/pointer_category.hpp>
#include <pika/functional/detail/tag_fallback_invoke.hpp>
#include <pika/parallel/algorithms/detail/distance.hpp>
#include <pika/parallel/util/result_types.hpp>

#include <algorithm>
#include <cstddef>
#include <cstring>    // for std::memmove
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

namespace pika::parallel::detail {
    template <typename Category, typename Enable = void>
    struct copy_helper;
    template <typename Category, typename Enable = void>
    struct copy_n_helper;

    template <typename Category, typename Enable = void>
    struct copy_synchronize_helper;

    template <typename Category, typename Enable = void>
    struct move_helper;
    template <typename Category, typename Enable = void>
    struct move_n_helper;

    ///////////////////////////////////////////////////////////////////////
    template <typename T>
    PIKA_FORCEINLINE std::enable_if_t<std::is_pointer<T>::value, char*>
    to_ptr(T ptr) noexcept
    {
        return const_cast<char*>(reinterpret_cast<char volatile const*>(ptr));
    }

    template <typename T>
    PIKA_FORCEINLINE std::enable_if_t<std::is_pointer<T>::value, char const*>
    to_const_ptr(T ptr) noexcept
    {
        return const_cast<char const*>(
            reinterpret_cast<char volatile const*>(ptr));
    }

    template <typename Iter>
    PIKA_FORCEINLINE std::enable_if_t<!std::is_pointer<Iter>::value, char*>
    to_ptr(Iter ptr) noexcept
    {
        static_assert(pika::traits::is_contiguous_iterator_v<Iter>,
            "optimized copy/move is possible for contiguous-iterators "
            "only");

        return const_cast<char*>(reinterpret_cast<char volatile const*>(&*ptr));
    }

    template <typename Iter>
    PIKA_FORCEINLINE
        std::enable_if_t<!std::is_pointer<Iter>::value, char const*>
        to_const_ptr(Iter ptr) noexcept
    {
        static_assert(pika::traits::is_contiguous_iterator_v<Iter>,
            "optimized copy/move is possible for contiguous-iterators "
            "only");

        return const_cast<char const*>(
            reinterpret_cast<char volatile const*>(&*ptr));
    }

    ///////////////////////////////////////////////////////////////////////
    template <typename InIter, typename OutIter>
    PIKA_FORCEINLINE static in_out_result<InIter, OutIter>
    copy_memmove(InIter first, std::size_t count, OutIter dest)
    {
        using data_type = typename std::iterator_traits<InIter>::value_type;

        char const* const first_ch = to_const_ptr(first);
        char* const dest_ch = to_ptr(dest);

        std::memmove(dest_ch, first_ch, count * sizeof(data_type));

        std::advance(first, count);
        std::advance(dest, count);
        return in_out_result<InIter, OutIter>{
            PIKA_MOVE(first), PIKA_MOVE(dest)};
    }

    ///////////////////////////////////////////////////////////////////////
    // Customization point for optimizing copy operations
    template <typename Category, typename Enable>
    struct copy_helper
    {
        template <typename InIter, typename Sent, typename OutIter>
        PIKA_HOST_DEVICE
            PIKA_FORCEINLINE static constexpr in_out_result<InIter, OutIter>
            call(InIter first, Sent last, OutIter dest)
        {
            while (first != last)
                *dest++ = *first++;
            return in_out_result<InIter, OutIter>{
                PIKA_MOVE(first), PIKA_MOVE(dest)};
        }
    };

    template <typename Dummy>
    struct copy_helper<pika::detail::trivially_copyable_pointer_tag, Dummy>
    {
        template <typename InIter, typename Sent, typename OutIter>
        PIKA_FORCEINLINE static in_out_result<InIter, OutIter>
        call(InIter first, Sent last, OutIter dest)
        {
            return copy_memmove(
                first, parallel::detail::distance(first, last), dest);
        }
    };

    template <typename InIter, typename Sent, typename OutIter>
    PIKA_FORCEINLINE constexpr in_out_result<InIter, OutIter>
    copy(InIter first, Sent last, OutIter dest)
    {
        using category = pika::detail::pointer_copy_category_t<
            std::decay_t<
                pika::detail::remove_const_iterator_value_type_t<InIter>>,
            std::decay_t<OutIter>>;
        return copy_helper<category>::call(first, last, dest);
    }

    // Customization point for optimizing copy_n operations
    template <typename Category, typename Enable>
    struct copy_n_helper
    {
        template <typename InIter, typename OutIter>
        PIKA_HOST_DEVICE
            PIKA_FORCEINLINE static constexpr in_out_result<InIter, OutIter>
            call(InIter first, std::size_t num, OutIter dest)
        {
            std::size_t count(num & std::size_t(-4));    // -V112
            for (std::size_t i = 0; i < count; (void) ++first, ++dest, i += 4)
            {
                *dest = *first;
                *++dest = *++first;
                *++dest = *++first;
                *++dest = *++first;
            }
            for (/**/; count < num; (void) ++first, ++dest, ++count)
            {
                *dest = *first;
            }
            return in_out_result<InIter, OutIter>{
                PIKA_MOVE(first), PIKA_MOVE(dest)};
        }
    };

    template <typename Dummy>
    struct copy_n_helper<pika::detail::trivially_copyable_pointer_tag, Dummy>
    {
        template <typename InIter, typename OutIter>
        PIKA_FORCEINLINE static in_out_result<InIter, OutIter>
        call(InIter first, std::size_t count, OutIter dest)
        {
            return copy_memmove(first, count, dest);
        }
    };

    template <typename ExPolicy>
    struct copy_n_t final
      : pika::functional::detail::tag_fallback<copy_n_t<ExPolicy>>
    {
    private:
        template <typename InIter, typename OutIter>
        friend PIKA_HOST_DEVICE
            PIKA_FORCEINLINE constexpr in_out_result<InIter, OutIter>
            tag_fallback_invoke(pika::parallel::detail::copy_n_t<ExPolicy>,
                InIter first, std::size_t count, OutIter dest)
        {
            using category = pika::detail::pointer_copy_category_t<
                std::decay_t<
                    pika::detail::remove_const_iterator_value_type_t<InIter>>,
                std::decay_t<OutIter>>;
            return copy_n_helper<category>::call(first, count, dest);
        }
    };

#if !defined(PIKA_COMPUTE_DEVICE_CODE)
    template <typename ExPolicy>
    inline constexpr copy_n_t<ExPolicy> copy_n = copy_n_t<ExPolicy>{};
#else
    template <typename ExPolicy, typename InIter, typename OutIter>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr in_out_result<InIter, OutIter>
    copy_n(InIter first, std::size_t count, OutIter dest)
    {
        return pika::parallel::detail::copy_n_t<ExPolicy>{}(first, count, dest);
    }
#endif

    // Customization point for copy-synchronize operations
    template <typename Category, typename Enable>
    struct copy_synchronize_helper
    {
        template <typename InIter, typename OutIter>
        PIKA_FORCEINLINE static constexpr void
        call(InIter const&, OutIter const&) noexcept
        {
            // do nothing by default (std::memmove is already synchronous)
        }
    };

    template <typename InIter, typename OutIter>
    PIKA_FORCEINLINE constexpr void
    copy_synchronize(InIter const& first, OutIter const& dest)
    {
        using category =
            pika::detail::pointer_copy_category_t<std::decay_t<InIter>,
                std::decay_t<OutIter>>;
        copy_synchronize_helper<category>::call(first, dest);
    }

    // Customization point for optimizing copy_n operations
    template <typename Category, typename Enable>
    struct move_helper
    {
        template <typename InIter, typename Sent, typename OutIter>
        PIKA_FORCEINLINE static constexpr in_out_result<InIter, OutIter>
        call(InIter first, Sent last, OutIter dest)
        {
            while (first != last)
            {
                // NOLINTNEXTLINE(bugprone-macro-repeated-side-effects)
                *dest++ = PIKA_MOVE(*first++);
            }

            return in_out_result<InIter, OutIter>{
                PIKA_MOVE(first), PIKA_MOVE(dest)};
        }
    };

    template <typename Dummy>
    struct move_helper<pika::detail::trivially_copyable_pointer_tag, Dummy>
    {
        template <typename InIter, typename Sent, typename OutIter>
        PIKA_FORCEINLINE static in_out_result<InIter, OutIter>
        call(InIter first, Sent last, OutIter dest)
        {
            return copy_memmove(
                first, parallel::detail::distance(first, last), dest);
        }
    };

    template <typename InIter, typename Sent, typename OutIter>
    PIKA_FORCEINLINE constexpr in_out_result<InIter, OutIter>
    move(InIter first, Sent last, OutIter dest)
    {
        using category =
            pika::detail::pointer_move_category_t<std::decay_t<InIter>,
                std::decay_t<OutIter>>;
        return move_helper<category>::call(first, last, dest);
    }

    // Customization point for optimizing copy_n operations
    template <typename Category, typename Enable>
    struct move_n_helper
    {
        template <typename InIter, typename OutIter>
        PIKA_FORCEINLINE static constexpr in_out_result<InIter, OutIter>
        call(InIter first, std::size_t num, OutIter dest)
        {
            std::size_t count(num & std::size_t(-4));    // -V112
            for (std::size_t i = 0; i < count; (void) ++first, ++dest, i += 4)
            {
                *dest = PIKA_MOVE(*first);
                // NOLINTNEXTLINE(bugprone-macro-repeated-side-effects)
                *++dest = PIKA_MOVE(*++first);
                // NOLINTNEXTLINE(bugprone-macro-repeated-side-effects)
                *++dest = PIKA_MOVE(*++first);
                // NOLINTNEXTLINE(bugprone-macro-repeated-side-effects)
                *++dest = PIKA_MOVE(*++first);
            }
            for (/**/; count < num; (void) ++first, ++dest, ++count)
            {
                *dest = PIKA_MOVE(*first);
            }
            return in_out_result<InIter, OutIter>{
                PIKA_MOVE(first), PIKA_MOVE(dest)};
        }
    };

    template <typename Dummy>
    struct move_n_helper<pika::detail::trivially_copyable_pointer_tag, Dummy>
    {
        template <typename InIter, typename OutIter>
        PIKA_FORCEINLINE static in_out_result<InIter, OutIter>
        call(InIter first, std::size_t count, OutIter dest)
        {
            return copy_memmove(first, count, dest);
        }
    };

    template <typename InIter, typename OutIter>
    PIKA_FORCEINLINE constexpr in_out_result<InIter, OutIter>
    move_n(InIter first, std::size_t count, OutIter dest)
    {
        using category =
            pika::detail::pointer_move_category_t<std::decay_t<InIter>,
                std::decay_t<OutIter>>;
        return move_n_helper<category>::call(first, count, dest);
    }
}    // namespace pika::parallel::detail
