//  Copyright (c) 2007-2016 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>
#include <pika/assert.hpp>
#include <pika/functional/invoke.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/type_support/equality.hpp>

#include <pika/parallel/algorithms/detail/is_negative.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <type_traits>
#include <utility>

namespace pika { namespace parallel { namespace detail {
    template <typename InputIterator, typename Distance>
    PIKA_HOST_DEVICE constexpr void
    advance_impl(InputIterator& i, Distance n, std::random_access_iterator_tag)
    {
        i += n;
    }

    template <typename InputIterator, typename Distance>
    PIKA_HOST_DEVICE constexpr void
    advance_impl(InputIterator& i, Distance n, std::bidirectional_iterator_tag)
    {
        if (n < 0)
        {
            while (n++)
                --i;
        }
        else
        {
            while (n--)
                ++i;
        }
    }

    template <typename InputIterator, typename Distance>
    PIKA_HOST_DEVICE constexpr void
    advance_impl(InputIterator& i, Distance n, std::input_iterator_tag)
    {
#if defined(PIKA_INTEL_VERSION)
#pragma warning(push)
#pragma warning(disable : 186)
#endif
        PIKA_ASSERT(n >= 0);
#if defined(PIKA_INTEL_VERSION)
#pragma warning(pop)
#endif
        while (n--)
            ++i;
    }

    template <typename InputIterator, typename Distance>
    PIKA_HOST_DEVICE constexpr void advance(InputIterator& i, Distance n)
    {
        advance_impl(i, n,
            typename std::iterator_traits<InputIterator>::iterator_category());
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterable, typename Enable = void>
    struct calculate_distance
    {
        template <typename T1, typename T2>
        PIKA_FORCEINLINE constexpr static std::size_t call(T1 t1, T2 t2)
        {
            return std::size_t(t2 - t1);
        }
    };

    template <typename Iterable>
    struct calculate_distance<Iterable,
        typename std::enable_if<
            pika::traits::is_iterator<Iterable>::value>::type>
    {
        template <typename Iter1, typename Iter2>
        PIKA_FORCEINLINE constexpr static std::size_t
        call(Iter1 iter1, Iter2 iter2)
        {
            return std::distance(iter1, iter2);
        }
    };

    template <typename Iterable>
    PIKA_FORCEINLINE constexpr std::size_t
    distance(Iterable iter1, Iterable iter2)
    {
        return calculate_distance<std::decay_t<Iterable>>::call(iter1, iter2);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterable, typename Enable = void>
    struct calculate_next
    {
        template <typename T, typename Stride>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr static T
        call(T t1, Stride offset)
        {
            return T(t1 + offset);
        }

        template <typename T, typename Stride>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr static T
        call(T t, std::size_t max_count, Stride& offset, std::true_type)
        {
            if (is_negative(offset))
            {
                offset = Stride(negate(
                    // NVCC seems to have a bug with std::min...
                    max_count < std::size_t(negate(offset)) ? max_count :
                                                              negate(offset)));
                return T(t + offset);
            }

            // NVCC seems to have a bug with std::min...
            offset =
                Stride(max_count < std::size_t(offset) ? max_count : offset);
            return T(t + offset);
        }

        template <typename T, typename Stride>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr static T
        call(T t, std::size_t max_count, Stride& offset, std::false_type)
        {
            // NVCC seems to have a bug with std::min...
            offset = max_count < offset ? max_count : offset;
            return T(t + offset);
        }

        template <typename T, typename Stride>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr static T
        call(T t, std::size_t max_count, Stride& offset)
        {
            return call(
                t, max_count, offset, typename std::is_signed<Stride>::type());
        }
    };

    template <typename Iterable>
    struct calculate_next<Iterable,
        typename std::enable_if<pika::traits::is_iterator<Iterable>::value &&
            !pika::traits::is_bidirectional_iterator<Iterable>::value>::type>
    {
        template <typename Iter, typename Stride>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr static Iter
        call(Iter iter, Stride offset)
        {
#if (defined(PIKA_HAVE_CUDA) && defined(__CUDACC__)) || defined(PIKA_HAVE_HIP)
            pika::parallel::detail::advance(iter, offset);
#else
            std::advance(iter, offset);
#endif
            return iter;
        }

        template <typename Iter, typename Stride>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr static Iter
        call(Iter iter, std::size_t max_count, Stride& offset)
        {
            // anything less than a bidirectional iterator does not support
            // negative offsets
            PIKA_ASSERT(!std::is_signed<Stride>::value || !is_negative(offset));

            // NVCC seems to have a bug with std::min...
            offset =
                Stride(max_count < std::size_t(offset) ? max_count : offset);

            // advance through the end or max number of elements
            for (Stride count = offset; count != 0; (void) ++iter, --count)
                /**/;

            return iter;
        }
    };

    template <typename Iterable>
    struct calculate_next<Iterable,
        typename std::enable_if<
            pika::traits::is_bidirectional_iterator<Iterable>::value>::type>
    {
        template <typename Iter, typename Stride>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr static Iter
        call(Iter iter, Stride offset)
        {
#if (defined(PIKA_HAVE_CUDA) && defined(__CUDACC__)) || defined(PIKA_HAVE_HIP)
            pika::parallel::detail::advance(iter, offset);
#else
            std::advance(iter, offset);
#endif
            return iter;
        }

        template <typename Iter, typename Stride>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr static Iter
        call(Iter iter, std::size_t max_count, Stride& offset, std::true_type)
        {
            // advance through the end or max number of elements
            if (!is_negative(offset))
            {
                // NVCC seems to have a bug with std::min...
                offset = Stride(
                    max_count < std::size_t(offset) ? max_count : offset);
#if (defined(PIKA_HAVE_CUDA) && defined(__CUDACC__)) || defined(PIKA_HAVE_HIP)
                pika::parallel::detail::advance(iter, offset);
#else
                std::advance(iter, offset);
#endif
            }
            else
            {
                offset = negate(Stride(
                    // NVCC seems to have a bug with std::min...
                    max_count < negate(offset) ? max_count : negate(offset)));
#if (defined(PIKA_HAVE_CUDA) && defined(__CUDACC__)) || defined(PIKA_HAVE_HIP)
                pika::parallel::detail::advance(iter, offset);
#else
                std::advance(iter, offset);
#endif
            }
            return iter;
        }

        template <typename Iter, typename Stride>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr static Iter
        call(Iter iter, std::size_t max_count, Stride& offset, std::false_type)
        {
            // advance through the end or max number of elements
            // NVCC seems to have a bug with std::min...
            offset =
                Stride(max_count < std::size_t(offset) ? max_count : offset);
#if (defined(PIKA_HAVE_CUDA) && defined(__CUDACC__)) || defined(PIKA_HAVE_HIP)
            pika::parallel::detail::advance(iter, offset);
#else
            std::advance(iter, offset);
#endif
            return iter;
        }

        template <typename Iter, typename Stride>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr static Iter
        call(Iter iter, std::size_t max_count, Stride& offset)
        {
            return call(iter, max_count, offset,
                typename std::is_signed<Stride>::type());
        }
    };

    template <typename Iterable, typename Stride>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr Iterable
    next(Iterable iter, Stride offset)
    {
        return calculate_next<std::decay_t<Iterable>>::call(iter, offset);
    }

    template <typename Iterable, typename Stride>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr Iterable
    next(Iterable iter, std::size_t max_count, Stride offset)
    {
        return calculate_next<std::decay_t<Iterable>>::call(
            iter, max_count, offset);
    }

    ///////////////////////////////////////////////////////////////////////////
    struct equal_to
    {
        template <typename T1, typename T2,
            typename Enable = std::enable_if_t<
                pika::detail::is_equality_comparable_with_v<T1, T2>>>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr auto
        operator()(T1&& t1, T2&& t2) const
        {
            return t1 == t2;
        }
    };

    struct not_equal_to
    {
        template <typename T1, typename T2,
            typename Enable = std::enable_if_t<
                pika::detail::is_equality_comparable_with_v<T1, T2>>>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr auto
        operator()(T1&& t1, T2&& t2) const
        {
            return t1 != t2;
        }
    };

    template <typename Value>
    struct compare_to
    {
        PIKA_HOST_DEVICE PIKA_FORCEINLINE compare_to(Value&& val)
          : value_(PIKA_MOVE(val))
        {
        }
        PIKA_HOST_DEVICE PIKA_FORCEINLINE compare_to(Value const& val)
          : value_(val)
        {
        }

        template <typename T>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr auto
        operator()(T const& t) const -> decltype(std::declval<Value>() == t)
        {
            return value_ == t;
        }

        Value value_;
    };

    ///////////////////////////////////////////////////////////////////////////
    struct less
    {
        template <typename T1, typename T2>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr auto
        operator()(T1&& t1, T2&& t2) const
        {
            return PIKA_FORWARD(T1, t1) < PIKA_FORWARD(T2, t2);
        }
    };

    struct greater
    {
        template <typename T1, typename T2>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr auto
        operator()(T1&& t1, T2&& t2) const
        {
            return PIKA_FORWARD(T1, t1) > PIKA_FORWARD(T2, t2);
        }
    };

    struct greater_equal
    {
        template <typename T1, typename T2>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr auto
        operator()(T1&& t1, T2&& t2) const
        {
            return PIKA_FORWARD(T1, t1) >= PIKA_FORWARD(T2, t2);
        }
    };

    struct less_equal
    {
        template <typename T1, typename T2>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr auto
        operator()(T1&& t1, T2&& t2) const
        {
            return PIKA_FORWARD(T1, t1) <= PIKA_FORWARD(T2, t2);
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct min_of
    {
        constexpr T operator()(T const& t1, T const& t2) const
        {
            // NVCC seems to have a bug with std::min...
            return t1 < t2 ? t1 : t2;
        }
    };

    template <typename T>
    struct max_of
    {
        constexpr T operator()(T const& t1, T const& t2) const
        {
            return (std::max)(t1, t2);
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    struct plus
    {
        template <typename T1, typename T2>
        constexpr auto operator()(T1 const& t1, T2 const& t2) const
            -> decltype(t1 + t2)
        {
            return t1 + t2;
        }
    };

    struct minus
    {
        template <typename T1, typename T2>
        constexpr auto operator()(T1 const& t1, T2 const& t2) const
            -> decltype(t1 - t2)
        {
            return t1 - t2;
        }
    };

    struct multiplies
    {
        template <typename T1, typename T2>
        constexpr auto operator()(T1 const& t1, T2 const& t2) const
            -> decltype(t1 * t2)
        {
            return t1 * t2;
        }
    };

    struct divides
    {
        template <typename T1, typename T2>
        constexpr auto operator()(T1 const& t1, T2 const& t2) const
            -> decltype(t1 / t2)
        {
            return t1 / t2;
        }
    };
}}}    // namespace pika::parallel::detail

namespace pika { namespace ranges {
    using equal_to = pika::parallel::detail::equal_to;
    using not_equal_to = pika::parallel::detail::not_equal_to;
    using less = pika::parallel::detail::less;
    using greater = pika::parallel::detail::greater;
    using greater_equal = pika::parallel::detail::greater_equal;
    using less_equal = pika::parallel::detail::less_equal;
}}    // namespace pika::ranges
