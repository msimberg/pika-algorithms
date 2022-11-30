//  Copyright (c) 2007-2021 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>

#if defined(PIKA_HAVE_DATAPAR)
#include <pika/execution/traits/is_execution_policy.hpp>
#include <pika/executors/datapar/execution_policy.hpp>
#include <pika/executors/execution_policy.hpp>
#include <pika/functional/tag_invoke.hpp>
#include <pika/parallel/algorithms/detail/predicates.hpp>
#include <pika/parallel/datapar/iterator_helpers.hpp>
#include <pika/parallel/util/loop.hpp>
#include <pika/parallel/util/vector_pack_alignment_size.hpp>
#include <pika/parallel/util/vector_pack_load_store.hpp>
#include <pika/parallel/util/vector_pack_type.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>

namespace pika::parallel::detail {
    template <typename ExPolicy, typename Vector>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE typename std::enable_if<
        pika::is_vectorpack_execution_policy<ExPolicy>::value,
        typename Vector::value_type>::type
    tag_invoke(extract_value_t<ExPolicy>, Vector const& value)
    {
        static_assert(traits::detail::is_scalar_vector_pack<Vector>::value,
            "this should be called with a scalar only");
        return value[0];
    }

    ///////////////////////////////////////////////////////////////////////
    template <typename ExPolicy, typename F, typename Vector>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE typename std::enable_if<
        pika::is_vectorpack_execution_policy<ExPolicy>::value,
        typename traits::detail::vector_pack_type<
            typename std::decay_t<Vector>::value_type, 1>::type>::type
    tag_invoke(accumulate_values_t<ExPolicy>, F&& f, Vector const& value)
    {
        using vector_type = std::decay_t<Vector>;
        using entry_type = typename vector_type::value_type;

        entry_type accum = value[0];
        for (size_t i = 1; i != value.size(); ++i)
        {
            accum = f(accum, entry_type(value[i]));
        }

        return typename traits::detail::vector_pack_type<entry_type, 1>::type(
            accum);
    }

    ///////////////////////////////////////////////////////////////////////
    template <typename ExPolicy, typename F, typename Vector, typename T>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE typename std::enable_if<
        pika::is_vectorpack_execution_policy<ExPolicy>::value,
        typename traits::detail::vector_pack_type<T, 1>::type>::type
    tag_invoke(
        accumulate_values_t<ExPolicy>, F&& f, Vector const& value, T accum)
    {
        for (size_t i = 0; i != value.size(); ++i)
        {
            accum = f(accum, T(value[i]));
        }

        return typename traits::detail::vector_pack_type<T, 1>::type(accum);
    }

    ///////////////////////////////////////////////////////////////////////
    template <typename Iter, typename Enable = void>
    struct loop_optimization_impl
    {
        template <typename Iter1, typename Sent1>
        PIKA_FORCEINLINE static constexpr bool call(Iter1 const&, Sent1 const&)
        {
            return false;
        }
    };

    template <typename Iter>
    struct loop_optimization_impl<Iter,
        typename std::enable_if<iterator_datapar_compatible<Iter>::value>::type>
    {
        template <typename Iter_, typename Sent_>
        static bool call(Iter_ const& first, Sent_ const& last)
        {
            typedef typename std::iterator_traits<Iter_>::value_type value_type;
            using V =
                typename traits::detail::vector_pack_type<value_type>::type;

            return traits::detail::vector_pack_size<V>::value <=
                (std::size_t) parallel::detail::distance(first, last);
        }
    };

    ///////////////////////////////////////////////////////////////////////
    // Helper class to repeatedly call a function starting from a given
    // iterator position.
    template <typename Iterator>
    struct datapar_loop_impl
    {
        using iterator_type = std::decay_t<Iterator>;
        typedef
            typename std::iterator_traits<iterator_type>::value_type value_type;

        using V = typename traits::detail::vector_pack_type<value_type>::type;

        template <typename Begin, typename End, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename std::enable_if<iterator_datapar_compatible<Begin>::value,
                Begin>::type
            call(Begin first, End last, F&& f)
        {
            while (!is_data_aligned(first) && first != last)
            {
                datapar_loop_step<Begin>::call1(f, first);
            }

            static std::size_t constexpr size =
                traits::detail::vector_pack_size<V>::value;

            End const lastV = last - (size + 1);
            while (first < lastV)
            {
                datapar_loop_step<Begin>::callv(f, first);
            }

            while (first != last)
            {
                datapar_loop_step<Begin>::call1(f, first);
            }

            return first;
        }

        template <typename Begin, typename End, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename std::enable_if<!iterator_datapar_compatible<Begin>::value,
                Begin>::type
            call(Begin it, End end, F&& f)
        {
            while (it != end)
            {
                datapar_loop_step<Begin>::call1(f, it);
            }
            return it;
        }

        template <typename Begin, typename End, typename CancelToken,
            typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename std::enable_if<iterator_datapar_compatible<Begin>::value,
                Begin>::type
            call(Begin first, End last, CancelToken& tok, F&& f)
        {
            while (!is_data_aligned(first) && first != last)
            {
                datapar_loop_step_tok<Begin>::call1(f, first);
                if (tok.was_cancelled())
                    return first;
                ++first;
            }

            static std::size_t constexpr size =
                traits::detail::vector_pack_size<V>::value;

            End const lastV = last - (size + 1);
            while (first < lastV)
            {
                std::size_t incr =
                    datapar_loop_step_tok<Begin>::callv(f, first);
                if (tok.was_cancelled())
                    return first;
                std::advance(first, incr);
            }

            while (first != last)
            {
                datapar_loop_step_tok<Begin>::call1(f, first);
                if (tok.was_cancelled())
                    return first;
                ++first;
            }

            return first;
        }

        template <typename Begin, typename End, typename CancelToken,
            typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename std::enable_if<!iterator_datapar_compatible<Begin>::value,
                Begin>::type
            call(Begin it, End end, CancelToken& tok, F&& f)
        {
            while (it != end)
            {
                datapar_loop_step_tok<Begin>::call1(f, it);
                if (tok.was_cancelled())
                    return it;
                ++it;
            }
            return it;
        }
    };

    ///////////////////////////////////////////////////////////////////////
    template <typename Iterator>
    struct datapar_loop_ind_impl
    {
        using iterator_type = std::decay_t<Iterator>;
        typedef
            typename std::iterator_traits<iterator_type>::value_type value_type;

        using V = typename traits::detail::vector_pack_type<value_type>::type;

        template <typename Begin, typename End, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename std::enable_if<iterator_datapar_compatible<Begin>::value,
                Begin>::type
            call(Begin first, End last, F&& f)
        {
            while (!is_data_aligned(first) && first != last)
            {
                datapar_loop_step_ind<Begin>::call1(f, first);
            }

            static std::size_t constexpr size =
                traits::detail::vector_pack_size<V>::value;

            End const lastV = last - (size + 1);
            while (first < lastV)
            {
                datapar_loop_step_ind<Begin>::callv(f, first);
            }

            while (first != last)
            {
                datapar_loop_step_ind<Begin>::call1(f, first);
            }

            return first;
        }

        template <typename Begin, typename End, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename std::enable_if<!iterator_datapar_compatible<Begin>::value,
                Begin>::type
            call(Begin it, End end, F&& f)
        {
            while (it != end)
            {
                datapar_loop_step_ind<Begin>::call1(f, it);
            }
            return it;
        }
    };

    ///////////////////////////////////////////////////////////////////////
    // Helper class to repeatedly call a function starting from a given
    // iterator position.
    template <typename VecOnly, typename Iter1, typename Iter2>
    struct datapar_loop2_impl;

    template <typename Iter1, typename Iter2>
    struct datapar_loop2_impl<std::true_type, Iter1, Iter2>
    {
        template <typename InIter1, typename InIter2, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static typename std::enable_if<
            iterators_datapar_compatible<InIter1, InIter2>::value &&
                iterator_datapar_compatible<InIter1>::value &&
                iterator_datapar_compatible<InIter2>::value,
            std::pair<InIter1, InIter2>>::type
        call(InIter1 it1, InIter1 last1, InIter2 it2, F&& f)
        {
            using iterator_type = std::decay_t<InIter1>;
            typedef typename std::iterator_traits<iterator_type>::value_type
                value_type;

            using V =
                typename traits::detail::vector_pack_type<value_type>::type;

            if (!detail::is_data_aligned(it1) || !detail::is_data_aligned(it2))
            {
                return std::make_pair(PIKA_MOVE(it1), PIKA_MOVE(it2));
            }

            static std::size_t constexpr size =
                traits::detail::vector_pack_size<V>::value;

            InIter1 const last1V = last1 - (size + 1);
            while (it1 < last1V)
            {
                datapar_loop_step2<InIter1, InIter2>::callv(f, it1, it2);
            }

            return std::make_pair(PIKA_MOVE(it1), PIKA_MOVE(it2));
        }

        template <typename InIter1, typename InIter2, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static typename std::enable_if<
            !iterators_datapar_compatible<InIter1, InIter2>::value ||
                !iterator_datapar_compatible<InIter1>::value ||
                !iterator_datapar_compatible<InIter2>::value,
            std::pair<InIter1, InIter2>>::type
        call(InIter1 it1, InIter1 /* last1 */, InIter2 it2, F&& /* f */)
        {
            return std::make_pair(PIKA_MOVE(it1), PIKA_MOVE(it2));
        }
    };

    template <typename Iter1, typename Iter2>
    struct loop2_impl;

    template <typename Iter1, typename Iter2>
    struct datapar_loop2_impl<std::false_type, Iter1, Iter2>
    {
        template <typename InIter1, typename InIter2, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static std::pair<InIter1, InIter2>
        call(InIter1 it1, InIter1 last1, InIter2 it2, F&& f)
        {
            return loop2_impl<InIter1, InIter2>::call(
                it1, last1, it2, PIKA_FORWARD(F, f));
        }
    };

    ///////////////////////////////////////////////////////////////////////
    template <typename Iterator>
    struct datapar_loop_n_impl
    {
        using iterator_type = std::decay_t<Iterator>;
        typedef
            typename std::iterator_traits<iterator_type>::value_type value_type;

        using V = typename traits::detail::vector_pack_type<value_type>::type;

        template <typename InIter, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename std::enable_if<iterator_datapar_compatible<InIter>::value,
                InIter>::type
            call(InIter first, std::size_t count, F&& f)
        {
            std::size_t len = count;

            for (/* */; !detail::is_data_aligned(first) && len != 0; --len)
            {
                datapar_loop_step<InIter>::call1(f, first);
            }

            static std::size_t constexpr size =
                traits::detail::vector_pack_size<V>::value;

            for (std::int64_t len_v = std::int64_t(len - (size + 1)); len_v > 0;
                 len_v -= size, len -= size)
            {
                datapar_loop_step<InIter>::callv(f, first);
            }

            for (/* */; len != 0; --len)
            {
                datapar_loop_step<InIter>::call1(f, first);
            }

            return first;
        }

        template <typename InIter, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename std::enable_if<!iterator_datapar_compatible<InIter>::value,
                InIter>::type
            call(InIter first, std::size_t count, F&& f)
        {
            for (/* */; count != 0; --count)
            {
                datapar_loop_step<InIter>::call1(f, first);
            }
            return first;
        }

        template <typename InIter, typename CancelToken, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename std::enable_if<iterator_datapar_compatible<InIter>::value,
                InIter>::type
            call(InIter first, std::size_t count, CancelToken& tok, F&& f)
        {
            std::size_t len = count;

            for (/* */; !detail::is_data_aligned(first) && len != 0; --len)
            {
                datapar_loop_step_tok<InIter>::call1(f, first);
                if (tok.was_cancelled())
                    return first;
                ++first;
            }

            static std::size_t constexpr size =
                traits::detail::vector_pack_size<V>::value;

            for (std::int64_t len_v = std::int64_t(len - (size + 1)); len_v > 0;
                 len_v -= size, len -= size)
            {
                std::size_t incr =
                    datapar_loop_step_tok<InIter>::callv(f, first);
                if (tok.was_cancelled())
                    return first;
                std::advance(first, incr);
            }

            for (/* */; len != 0; --len)
            {
                datapar_loop_step_tok<InIter>::call1(f, first);
                if (tok.was_cancelled())
                    return first;
                ++first;
            }

            return first;
        }

        template <typename InIter, typename CancelToken, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename std::enable_if<!iterator_datapar_compatible<InIter>::value,
                InIter>::type
            call(InIter first, std::size_t count, CancelToken& tok, F&& f)
        {
            for (/* */; count != 0; --count)
            {
                datapar_loop_step_tok<InIter>::call1(f, first);
                if (tok.was_cancelled())
                    return first;
                ++first;
            }
            return first;
        }
    };

    ///////////////////////////////////////////////////////////////////////
    template <typename Iterator>
    struct datapar_loop_n_ind_impl
    {
        using iterator_type = std::decay_t<Iterator>;
        typedef
            typename std::iterator_traits<iterator_type>::value_type value_type;

        using V = typename traits::detail::vector_pack_type<value_type>::type;

        template <typename InIter, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename std::enable_if<iterator_datapar_compatible<InIter>::value,
                InIter>::type
            call(InIter first, std::size_t count, F&& f)
        {
            std::size_t len = count;

            for (/* */; !detail::is_data_aligned(first) && len != 0; --len)
            {
                datapar_loop_step_ind<InIter>::call1(f, first);
            }

            static std::size_t constexpr size =
                traits::detail::vector_pack_size<V>::value;

            for (std::int64_t len_v = std::int64_t(len - (size + 1)); len_v > 0;
                 len_v -= size, len -= size)
            {
                datapar_loop_step_ind<InIter>::callv(f, first);
            }

            for (/* */; len != 0; --len)
            {
                datapar_loop_step_ind<InIter>::call1(f, first);
            }

            return first;
        }

        template <typename InIter, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename std::enable_if<!iterator_datapar_compatible<InIter>::value,
                InIter>::type
            call(InIter first, std::size_t count, F&& f)
        {
            for (/* */; count != 0; --count)
            {
                datapar_loop_step_ind<InIter>::call1(f, first);
            }
            return first;
        }
    };

    ///////////////////////////////////////////////////////////////////////
    template <typename Iterator>
    struct datapar_loop_idx_n_impl
    {
        using iterator_type = std::decay_t<Iterator>;
        typedef
            typename std::iterator_traits<iterator_type>::value_type value_type;

        using V = typename traits::detail::vector_pack_type<value_type>::type;

        template <typename Iter, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static Iter
        call(std::size_t base_idx, Iter it, std::size_t count, F&& f)
        {
            std::size_t len = count;

            for (/* */; !detail::is_data_aligned(it) && len != 0; --len)
            {
                datapar_loop_idx_step<Iter>::call1(f, it, base_idx);
                ++it;
                ++base_idx;
            }

            static std::size_t constexpr size =
                traits::detail::vector_pack_size<V>::value;

            for (std::int64_t len_v = std::int64_t(len - (size + 1)); len_v > 0;
                 len_v -= size, len -= size)
            {
                std::size_t incr =
                    datapar_loop_idx_step<Iter>::callv(f, it, base_idx);
                std::advance(it, incr);
                base_idx += incr;
            }

            for (/* */; len != 0; --len)
            {
                datapar_loop_idx_step<Iter>::call1(f, it, base_idx);
                ++it;
                ++base_idx;
            }
            return it;
        }

        template <typename Iter, typename CancelToken, typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static Iter call(std::size_t base_idx,
            Iter it, std::size_t count, CancelToken& tok, F&& f)
        {
            std::size_t len = count;

            for (/* */; !detail::is_data_aligned(it) && len != 0; --len)
            {
                datapar_loop_idx_step<Iter>::call1(f, it, base_idx);
                if (tok.was_cancelled(base_idx))
                    return it;
                ++it;
                ++base_idx;
            }

            static std::size_t constexpr size =
                traits::detail::vector_pack_size<V>::value;

            for (std::int64_t len_v = std::int64_t(len - (size + 1)); len_v > 0;
                 len_v -= size, len -= size)
            {
                datapar_loop_idx_step<Iter>::callv(f, it, base_idx);
                if (tok.was_cancelled(base_idx))
                    return it;
                std::advance(it, size);
                base_idx += size;
            }

            for (/* */; len != 0; --len)
            {
                datapar_loop_idx_step<Iter>::call1(f, it, base_idx);
                if (tok.was_cancelled(base_idx))
                    return it;
                ++it;
                ++base_idx;
            }
            return it;
        }
    };

    template <typename ExPolicy, typename F, typename Iter1, typename Iter2,
        typename U = typename std::enable_if<
            pika::is_vectorpack_execution_policy<ExPolicy>::value>::type>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE auto tag_invoke(
        loop_step_t<ExPolicy>, std::false_type, F&& f, Iter1& it1, Iter2& it2)
        -> decltype(datapar_loop_step2<Iter1, Iter2>::call1(
            PIKA_FORWARD(F, f), it1, it2))
    {
        return datapar_loop_step2<Iter1, Iter2>::call1(
            PIKA_FORWARD(F, f), it1, it2);
    }

    template <typename ExPolicy, typename F, typename Iter1, typename Iter2,
        typename U = typename std::enable_if<
            pika::is_vectorpack_execution_policy<ExPolicy>::value>::type>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE auto tag_invoke(
        loop_step_t<ExPolicy>, std::true_type, F&& f, Iter1& it1, Iter2& it2)
        -> decltype(datapar_loop_step2<Iter1, Iter2>::callv(
            PIKA_FORWARD(F, f), it1, it2))
    {
        return datapar_loop_step2<Iter1, Iter2>::callv(
            PIKA_FORWARD(F, f), it1, it2);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename ExPolicy, typename Iter, typename Sent>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE typename std::enable_if<
        pika::is_vectorpack_execution_policy<ExPolicy>::value, bool>::type
    tag_invoke(loop_optimization_t<ExPolicy>, Iter first1, Sent last1)
    {
        return loop_optimization_impl<Iter>::call(first1, last1);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Begin, typename End, typename F>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE Begin tag_invoke(
        loop_t, pika::execution::simd_policy, Begin begin, End end, F&& f)
    {
        return datapar_loop_impl<Begin>::call(begin, end, PIKA_FORWARD(F, f));
    }

    template <typename Begin, typename End, typename F>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE Begin tag_invoke(
        loop_t, pika::execution::simd_task_policy, Begin begin, End end, F&& f)
    {
        return datapar_loop_impl<Begin>::call(begin, end, PIKA_FORWARD(F, f));
    }

    template <typename Begin, typename End, typename CancelToken, typename F>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE Begin tag_invoke(loop_t,
        pika::execution::simd_policy, Begin begin, End end, CancelToken& tok,
        F&& f)
    {
        return datapar_loop_impl<Begin>::call(
            begin, end, tok, PIKA_FORWARD(F, f));
    }

    template <typename Begin, typename End, typename CancelToken, typename F>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE Begin tag_invoke(loop_t,
        pika::execution::simd_task_policy, Begin begin, End end,
        CancelToken& tok, F&& f)
    {
        return datapar_loop_impl<Begin>::call(
            begin, end, tok, PIKA_FORWARD(F, f));
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Begin, typename End, typename F>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE Begin tag_invoke(
        loop_ind_t, pika::execution::simd_policy, Begin begin, End end, F&& f)
    {
        return datapar_loop_ind_impl<Begin>::call(
            begin, end, PIKA_FORWARD(F, f));
    }

    template <typename Begin, typename End, typename F>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE Begin tag_invoke(loop_ind_t,
        pika::execution::simd_task_policy, Begin begin, End end, F&& f)
    {
        return datapar_loop_ind_impl<Begin>::call(
            begin, end, PIKA_FORWARD(F, f));
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename ExPolicy, typename VecOnly, typename Iter1,
        typename Iter2, typename F>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE typename std::enable_if<
        pika::is_vectorpack_execution_policy<ExPolicy>::value,
        std::pair<Iter1, Iter2>>::type
    tag_invoke(loop2_t<ExPolicy>, VecOnly&&, Iter1 first1, Iter1 last1,
        Iter2 first2, F&& f)
    {
        return datapar_loop2_impl<VecOnly, Iter1, Iter2>::call(
            first1, last1, first2, PIKA_FORWARD(F, f));
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename ExPolicy, typename Iter, typename F>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr typename std::enable_if<
        pika::is_vectorpack_execution_policy<ExPolicy>::value, Iter>::type
    tag_invoke(loop_n_t<ExPolicy>, Iter it, std::size_t count, F&& f)
    {
        return datapar_loop_n_impl<Iter>::call(it, count, PIKA_FORWARD(F, f));
    }

    template <typename ExPolicy, typename Iter, typename CancelToken,
        typename F>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr typename std::enable_if<
        pika::is_vectorpack_execution_policy<ExPolicy>::value, Iter>::type
    tag_invoke(
        loop_n_t<ExPolicy>, Iter it, std::size_t count, CancelToken& tok, F&& f)
    {
        return datapar_loop_n_impl<Iter>::call(
            it, count, tok, PIKA_FORWARD(F, f));
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename ExPolicy, typename Iter, typename F>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr typename std::enable_if<
        pika::is_vectorpack_execution_policy<ExPolicy>::value, Iter>::type
    tag_invoke(loop_n_ind_t<ExPolicy>, Iter it, std::size_t count, F&& f)
    {
        return datapar_loop_n_ind_impl<Iter>::call(
            it, count, PIKA_FORWARD(F, f));
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename ExPolicy, typename Iter, typename F>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr typename std::enable_if<
        pika::is_vectorpack_execution_policy<ExPolicy>::value, Iter>::type
    tag_invoke(loop_idx_n_t<ExPolicy>, std::size_t base_idx, Iter it,
        std::size_t count, F&& f)
    {
        return datapar_loop_idx_n_impl<Iter>::call(
            base_idx, it, count, PIKA_FORWARD(F, f));
    }

    template <typename ExPolicy, typename Iter, typename CancelToken,
        typename F>
    PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr typename std::enable_if<
        pika::is_vectorpack_execution_policy<ExPolicy>::value, Iter>::type
    tag_invoke(loop_idx_n_t<ExPolicy>, std::size_t base_idx, Iter it,
        std::size_t count, CancelToken& tok, F&& f)
    {
        return datapar_loop_idx_n_impl<Iter>::call(
            base_idx, it, count, tok, PIKA_FORWARD(F, f));
    }
}    // namespace pika::parallel::detail

#endif
