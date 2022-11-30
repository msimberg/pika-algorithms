//  Copyright (c) 2016 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>

#if defined(PIKA_HAVE_DATAPAR)
#include <pika/assert.hpp>
#include <pika/functional/detail/invoke.hpp>
#include <pika/functional/invoke_result.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/parallel/util/vector_pack_alignment_size.hpp>
#include <pika/parallel/util/vector_pack_load_store.hpp>
#include <pika/parallel/util/vector_pack_type.hpp>

#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

namespace pika::parallel::detail {
    template <typename Iter>
    struct is_data_aligned_impl
    {
        static PIKA_FORCEINLINE bool call(Iter const& it)
        {
            using value_type = typename std::iterator_traits<Iter>::value_type;
            return (reinterpret_cast<std::uintptr_t>(std::addressof(*it)) &
                       (traits::detail::vector_pack_alignment<
                            value_type>::value -
                           1)) == 0;
        }
    };

    template <typename Iter>
    PIKA_FORCEINLINE bool is_data_aligned(Iter const& it)
    {
        return is_data_aligned_impl<Iter>::call(it);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iter1, typename Iter2>
    struct iterators_datapar_compatible_impl
    {
        using iterator1_type = std::decay_t<Iter1>;
        using iterator2_type = std::decay_t<Iter2>;

        typedef typename std::iterator_traits<iterator1_type>::value_type
            value1_type;
        typedef typename std::iterator_traits<iterator2_type>::value_type
            value2_type;

        typedef std::integral_constant<bool,
            traits::detail::vector_pack_size<value1_type>::value ==
                    traits::detail::vector_pack_size<value2_type>::value &&
                traits::detail::vector_pack_alignment<value1_type>::value ==
                    traits::detail::vector_pack_alignment<value2_type>::value>
            type;
    };

    template <typename Iter1, typename Iter2>
    struct iterators_datapar_compatible
      : iterators_datapar_compatible_impl<Iter1, Iter2>::type
    {
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iter, typename Enable = void>
    struct iterator_datapar_compatible_impl
      : std::is_arithmetic<typename std::iterator_traits<Iter>::value_type>
    {
    };

    template <typename Iter, typename Enable = void>
    struct iterator_datapar_compatible : std::false_type
    {
    };

    template <typename Iter>
    struct iterator_datapar_compatible<Iter,
        std::enable_if_t<pika::traits::is_random_access_iterator_v<Iter>>>
      : iterator_datapar_compatible_impl<std::decay_t<Iter>>::type
    {
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iter, typename V, typename Enable = void>
    struct store_on_exit
    {
        using pack_type = V;
        using value_type = typename std::iterator_traits<Iter>::value_type;

        store_on_exit(Iter const& iter)
          : value_(
                traits::detail::vector_pack_load<V, value_type>::aligned(iter))
          , iter_(iter)
        {
        }
        ~store_on_exit()
        {
            traits::detail::vector_pack_store<V, value_type>::aligned(
                value_, iter_);
        }

        pack_type* operator&()
        {
            return &value_;
        }
        pack_type const* operator&() const
        {
            return &value_;
        }

        pack_type value_;
        Iter iter_;
    };

    template <typename Iter, typename V>
    struct store_on_exit<Iter, V,
        typename std::enable_if<std::is_const<
            typename std::iterator_traits<Iter>::value_type>::value>::type>
    {
        using pack_type = V;
        using value_type = typename std::iterator_traits<Iter>::value_type;

        store_on_exit(Iter const& iter)
          : value_(
                traits::detail::vector_pack_load<V, value_type>::aligned(iter))
        {
        }

        pack_type* operator&()
        {
            return &value_;
        }
        pack_type const* operator&() const
        {
            return &value_;
        }

        pack_type value_;
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iter, typename V, typename Enable = void>
    struct store_on_exit_unaligned
    {
        using pack_type = V;
        using value_type = typename std::iterator_traits<Iter>::value_type;

        store_on_exit_unaligned(Iter const& iter)
          : value_(traits::detail::vector_pack_load<V, value_type>::unaligned(
                iter))
          , iter_(iter)
        {
        }
        ~store_on_exit_unaligned()
        {
            traits::detail::vector_pack_store<V, value_type>::unaligned(
                value_, iter_);
        }

        pack_type* operator&()
        {
            return &value_;
        }
        pack_type const* operator&() const
        {
            return &value_;
        }

        pack_type value_;
        Iter iter_;
    };

    template <typename Iter, typename V>
    struct store_on_exit_unaligned<Iter, V,
        typename std::enable_if<std::is_const<
            typename std::iterator_traits<Iter>::value_type>::value>::type>
    {
        using pack_type = V;
        using value_type = typename std::iterator_traits<Iter>::value_type;

        store_on_exit_unaligned(Iter const& iter)
          : value_(traits::detail::vector_pack_load<V, value_type>::unaligned(
                iter))
        {
        }

        pack_type* operator&()
        {
            return &value_;
        }
        pack_type const* operator&() const
        {
            return &value_;
        }

        pack_type value_;
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iter, typename Enable = void>
    struct datapar_loop_step
    {
        using value_type = typename std::iterator_traits<Iter>::value_type;

        typedef
            typename traits::detail::vector_pack_type<value_type, 1>::type V1;
        using V = typename traits::detail::vector_pack_type<value_type>::type;

        template <typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void call1(F&& f, Iter& it)
        {
            V1 tmp(
                traits::detail::vector_pack_load<V1, value_type>::aligned(it));
            PIKA_INVOKE(f, &tmp);
            traits::detail::vector_pack_store<V1, value_type>::aligned(tmp, it);
            ++it;
        }

        template <typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void callv(F&& f, Iter& it)
        {
            V tmp(traits::detail::vector_pack_load<V, value_type>::aligned(it));
            PIKA_INVOKE(f, &tmp);
            traits::detail::vector_pack_store<V, value_type>::aligned(tmp, it);
            std::advance(it, traits::detail::vector_pack_size<V>::value);
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iter, typename Enable = void>
    struct datapar_loop_step_ind
    {
        using value_type = typename std::iterator_traits<Iter>::value_type;

        typedef
            typename traits::detail::vector_pack_type<value_type, 1>::type V1;
        using V = typename traits::detail::vector_pack_type<value_type>::type;

        template <typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void call1(F&& f, Iter& it)
        {
            V1 tmp(
                traits::detail::vector_pack_load<V1, value_type>::aligned(it));
            PIKA_INVOKE(f, tmp);
            traits::detail::vector_pack_store<V1, value_type>::aligned(tmp, it);
            ++it;
        }

        template <typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void callv(F&& f, Iter& it)
        {
            V tmp(traits::detail::vector_pack_load<V, value_type>::aligned(it));
            PIKA_INVOKE(f, tmp);
            traits::detail::vector_pack_store<V, value_type>::aligned(tmp, it);
            std::advance(it, traits::detail::vector_pack_size<V>::value);
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iter>
    struct datapar_loop_idx_step
    {
        using value_type = typename std::iterator_traits<Iter>::value_type;

        typedef
            typename traits::detail::vector_pack_type<value_type, 1>::type V1;
        using V = typename traits::detail::vector_pack_type<value_type>::type;

        template <typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call1(F&& f, Iter& it, std::size_t base_idx)
        {
            V1 tmp(
                traits::detail::vector_pack_load<V1, value_type>::aligned(it));
            PIKA_INVOKE(f, tmp, base_idx);
            traits::detail::vector_pack_store<V1, value_type>::aligned(tmp, it);
        }

        template <typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        callv(F&& f, Iter& it, std::size_t base_idx)
        {
            V tmp(traits::detail::vector_pack_load<V, value_type>::aligned(it));
            PIKA_INVOKE(f, tmp, base_idx);
            traits::detail::vector_pack_store<V, value_type>::aligned(tmp, it);
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iter, typename Enable = void>
    struct datapar_loop_step_tok
    {
        using value_type = typename std::iterator_traits<Iter>::value_type;

        typedef
            typename traits::detail::vector_pack_type<value_type, 1>::type V1;
        using V = typename traits::detail::vector_pack_type<value_type>::type;

        template <typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void call1(F&& f, Iter& it)
        {
            V1 tmp(
                traits::detail::vector_pack_load<V1, value_type>::aligned(it));
            PIKA_INVOKE(f, &tmp);
            traits::detail::vector_pack_store<V1, value_type>::aligned(tmp, it);
        }

        template <typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static std::size_t
        callv(F&& f, Iter& it)
        {
            V tmp(traits::detail::vector_pack_load<V, value_type>::aligned(it));
            PIKA_INVOKE(f, &tmp);
            traits::detail::vector_pack_store<V, value_type>::aligned(tmp, it);
            return traits::detail::vector_pack_size<V>::value;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename V1, typename V2>
    struct invoke_vectorized_in2
    {
        template <typename F, typename Iter1, typename Iter2>
        static typename pika::util::detail::invoke_result<F, V1*, V2*>::type
        call_aligned(F&& f, Iter1& it1, Iter2& it2)
        {
            static_assert(traits::detail::vector_pack_size<V1>::value ==
                    traits::detail::vector_pack_size<V2>::value,
                "the sizes of the vector-packs should be equal");

            typedef
                typename std::iterator_traits<Iter1>::value_type value_type1;
            typedef
                typename std::iterator_traits<Iter2>::value_type value_type2;

            V1 tmp1(traits::detail::vector_pack_load<V1, value_type1>::aligned(
                it1));
            V2 tmp2(traits::detail::vector_pack_load<V2, value_type2>::aligned(
                it2));

            std::advance(it1, traits::detail::vector_pack_size<V1>::value);
            std::advance(it2, traits::detail::vector_pack_size<V2>::value);

            return PIKA_INVOKE(PIKA_FORWARD(F, f), &tmp1, &tmp2);
        }

        template <typename F, typename Iter1, typename Iter2>
        static typename pika::util::detail::invoke_result<F, V1*, V2*>::type
        call_unaligned(F&& f, Iter1& it1, Iter2& it2)
        {
            static_assert(traits::detail::vector_pack_size<V1>::value ==
                    traits::detail::vector_pack_size<V2>::value,
                "the sizes of the vector-packs should be equal");

            typedef
                typename std::iterator_traits<Iter1>::value_type value_type1;
            typedef
                typename std::iterator_traits<Iter2>::value_type value_type2;

            V1 tmp1(
                traits::detail::vector_pack_load<V1, value_type1>::unaligned(
                    it1));
            V2 tmp2(
                traits::detail::vector_pack_load<V2, value_type2>::unaligned(
                    it2));

            std::advance(it1, traits::detail::vector_pack_size<V1>::value);
            std::advance(it2, traits::detail::vector_pack_size<V2>::value);

            return PIKA_INVOKE(PIKA_FORWARD(F, f), &tmp1, &tmp2);
        }
    };

    template <typename Iter1, typename Iter2>
    struct datapar_loop_step2
    {
        using value1_type = typename std::iterator_traits<Iter1>::value_type;
        using value2_type = typename std::iterator_traits<Iter2>::value_type;

        typedef
            typename traits::detail::vector_pack_type<value1_type, 1>::type V11;
        typedef
            typename traits::detail::vector_pack_type<value2_type, 1>::type V12;

        using V1 = typename traits::detail::vector_pack_type<value1_type>::type;
        using V2 = typename traits::detail::vector_pack_type<value2_type>::type;

        template <typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename pika::util::detail::invoke_result<F, V11*, V12*>::type
            call1(F&& f, Iter1& it1, Iter2& it2)
        {
            return invoke_vectorized_in2<V11, V12>::call_aligned(
                PIKA_FORWARD(F, f), it1, it2);
        }

        template <typename F>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static
            typename pika::util::detail::invoke_result<F, V1*, V2*>::type
            callv(F&& f, Iter1& it1, Iter2& it2)
        {
            if (!is_data_aligned(it1) || !is_data_aligned(it2))
            {
                return invoke_vectorized_in2<V1, V2>::call_unaligned(
                    PIKA_FORWARD(F, f), it1, it2);
            }

            return invoke_vectorized_in2<V1, V2>::call_aligned(
                PIKA_FORWARD(F, f), it1, it2);
        }
    };

    ///////////////////////////////////////////////////////////////////////
    template <typename V>
    struct invoke_vectorized_inout1
    {
        template <typename F, typename InIter, typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call_aligned(F&& f, InIter& it, OutIter& dest)
        {
            typedef
                typename std::iterator_traits<InIter>::value_type value_type;

            V tmp(traits::detail::vector_pack_load<V, value_type>::aligned(it));

            auto ret = PIKA_INVOKE(f, &tmp);
            traits::detail::vector_pack_store<decltype(ret),
                value_type>::aligned(ret, dest);

            std::advance(it, traits::detail::vector_pack_size<V>::value);
            std::advance(dest, ret.size());
        }

        template <typename F, typename InIter, typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call_unaligned(F&& f, InIter& it, OutIter& dest)
        {
            typedef
                typename std::iterator_traits<InIter>::value_type value_type;

            V tmp(
                traits::detail::vector_pack_load<V, value_type>::unaligned(it));

            auto ret = PIKA_INVOKE(f, &tmp);
            traits::detail::vector_pack_store<decltype(ret),
                value_type>::unaligned(ret, dest);

            std::advance(it, traits::detail::vector_pack_size<V>::value);
            std::advance(dest, ret.size());
        }
    };

    template <typename V>
    struct invoke_vectorized_inout1_ind
    {
        template <typename F, typename InIter, typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call_aligned(F&& f, InIter& it, OutIter& dest)
        {
            typedef
                typename std::iterator_traits<InIter>::value_type value_type;

            V tmp(traits::detail::vector_pack_load<V, value_type>::aligned(it));

            auto ret = PIKA_INVOKE(f, tmp);
            traits::detail::vector_pack_store<decltype(ret),
                value_type>::aligned(ret, dest);

            std::advance(it, traits::detail::vector_pack_size<V>::value);
            std::advance(dest, ret.size());
        }

        template <typename F, typename InIter, typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call_unaligned(F&& f, InIter& it, OutIter& dest)
        {
            typedef
                typename std::iterator_traits<InIter>::value_type value_type;

            V tmp(
                traits::detail::vector_pack_load<V, value_type>::unaligned(it));

            auto ret = PIKA_INVOKE(f, tmp);
            traits::detail::vector_pack_store<decltype(ret),
                value_type>::unaligned(ret, dest);

            std::advance(it, traits::detail::vector_pack_size<V>::value);
            std::advance(dest, ret.size());
        }
    };

    template <typename V1, typename V2>
    struct invoke_vectorized_inout2
    {
        template <typename F, typename InIter1, typename InIter2,
            typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call_aligned(F&& f, InIter1& it1, InIter2& it2, OutIter& dest)
        {
            static_assert(traits::detail::vector_pack_size<V1>::value ==
                    traits::detail::vector_pack_size<V2>::value,
                "the sizes of the vector-packs should be equal");

            typedef
                typename std::iterator_traits<InIter1>::value_type value_type1;
            typedef
                typename std::iterator_traits<InIter2>::value_type value_type2;

            V1 tmp1(traits::detail::vector_pack_load<V1, value_type1>::aligned(
                it1));
            V2 tmp2(traits::detail::vector_pack_load<V2, value_type2>::aligned(
                it2));

            auto ret = PIKA_INVOKE(f, &tmp1, &tmp2);
            traits::detail::vector_pack_store<decltype(ret),
                value_type1>::aligned(ret, dest);

            std::advance(it1, traits::detail::vector_pack_size<V1>::value);
            std::advance(it2, traits::detail::vector_pack_size<V2>::value);
            std::advance(dest, ret.size());
        }

        template <typename F, typename InIter1, typename InIter2,
            typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call_unaligned(F&& f, InIter1& it1, InIter2& it2, OutIter& dest)
        {
            static_assert(traits::detail::vector_pack_size<V1>::value ==
                    traits::detail::vector_pack_size<V2>::value,
                "the sizes of the vector-packs should be equal");

            typedef
                typename std::iterator_traits<InIter1>::value_type value_type1;
            typedef
                typename std::iterator_traits<InIter2>::value_type value_type2;

            V1 tmp1(
                traits::detail::vector_pack_load<V1, value_type1>::unaligned(
                    it1));
            V2 tmp2(
                traits::detail::vector_pack_load<V2, value_type2>::unaligned(
                    it2));

            auto ret = PIKA_INVOKE(f, &tmp1, &tmp2);
            traits::detail::vector_pack_store<decltype(ret),
                value_type1>::unaligned(ret, dest);

            std::advance(it1, traits::detail::vector_pack_size<V1>::value);
            std::advance(it2, traits::detail::vector_pack_size<V2>::value);
            std::advance(dest, ret.size());
        }
    };

    template <typename V1, typename V2>
    struct invoke_vectorized_inout2_ind
    {
        template <typename F, typename InIter1, typename InIter2,
            typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call_aligned(F&& f, InIter1& it1, InIter2& it2, OutIter& dest)
        {
            static_assert(traits::detail::vector_pack_size<V1>::value ==
                    traits::detail::vector_pack_size<V2>::value,
                "the sizes of the vector-packs should be equal");

            typedef
                typename std::iterator_traits<InIter1>::value_type value_type1;
            typedef
                typename std::iterator_traits<InIter2>::value_type value_type2;

            V1 tmp1(traits::detail::vector_pack_load<V1, value_type1>::aligned(
                it1));
            V2 tmp2(traits::detail::vector_pack_load<V2, value_type2>::aligned(
                it2));

            auto ret = PIKA_INVOKE(f, tmp1, tmp2);
            traits::detail::vector_pack_store<decltype(ret),
                value_type1>::aligned(ret, dest);

            std::advance(it1, traits::detail::vector_pack_size<V1>::value);
            std::advance(it2, traits::detail::vector_pack_size<V2>::value);
            std::advance(dest, ret.size());
        }

        template <typename F, typename InIter1, typename InIter2,
            typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call_unaligned(F&& f, InIter1& it1, InIter2& it2, OutIter& dest)
        {
            static_assert(traits::detail::vector_pack_size<V1>::value ==
                    traits::detail::vector_pack_size<V2>::value,
                "the sizes of the vector-packs should be equal");

            typedef
                typename std::iterator_traits<InIter1>::value_type value_type1;
            typedef
                typename std::iterator_traits<InIter2>::value_type value_type2;

            V1 tmp1(
                traits::detail::vector_pack_load<V1, value_type1>::unaligned(
                    it1));
            V2 tmp2(
                traits::detail::vector_pack_load<V2, value_type2>::unaligned(
                    it2));

            auto ret = PIKA_INVOKE(f, tmp1, tmp2);
            traits::detail::vector_pack_store<decltype(ret),
                value_type1>::unaligned(ret, dest);

            std::advance(it1, traits::detail::vector_pack_size<V1>::value);
            std::advance(it2, traits::detail::vector_pack_size<V2>::value);
            std::advance(dest, ret.size());
        }
    };

    struct datapar_transform_loop_step
    {
        template <typename F, typename InIter, typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call1(F&& f, InIter& it, OutIter& dest)
        {
            typedef
                typename std::iterator_traits<InIter>::value_type value_type;

            typedef
                typename traits::detail::vector_pack_type<value_type, 1>::type
                    V1;

            invoke_vectorized_inout1<V1>::call_unaligned(
                PIKA_FORWARD(F, f), it, dest);
        }

        template <typename F, typename InIter1, typename InIter2,
            typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call1(F&& f, InIter1& it1, InIter2& it2, OutIter& dest)
        {
            typedef
                typename std::iterator_traits<InIter1>::value_type value_type1;
            typedef
                typename std::iterator_traits<InIter2>::value_type value_type2;

            typedef
                typename traits::detail::vector_pack_type<value_type1, 1>::type
                    V1;
            typedef
                typename traits::detail::vector_pack_type<value_type2, 1>::type
                    V2;

            invoke_vectorized_inout2<V1, V2>::call_unaligned(
                PIKA_FORWARD(F, f), it1, it2, dest);
        }

        ///////////////////////////////////////////////////////////////////
        template <typename F, typename InIter, typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        callv(F&& f, InIter& it, OutIter& dest)
        {
            typedef
                typename std::iterator_traits<InIter>::value_type value_type;

            using V =
                typename traits::detail::vector_pack_type<value_type>::type;

            PIKA_ASSERT(is_data_aligned(it) && is_data_aligned(dest));
            invoke_vectorized_inout1<V>::call_aligned(
                PIKA_FORWARD(F, f), it, dest);
        }

        template <typename F, typename InIter1, typename InIter2,
            typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        callv(F&& f, InIter1& it1, InIter2& it2, OutIter& dest)
        {
            typedef
                typename std::iterator_traits<InIter1>::value_type value1_type;
            typedef
                typename std::iterator_traits<InIter2>::value_type value2_type;

            using V1 =
                typename traits::detail::vector_pack_type<value1_type>::type;
            using V2 =
                typename traits::detail::vector_pack_type<value2_type>::type;

            PIKA_ASSERT(is_data_aligned(it1) && is_data_aligned(it2) &&
                is_data_aligned(dest));
            invoke_vectorized_inout2<V1, V2>::call_aligned(
                PIKA_FORWARD(F, f), it1, it2, dest);
        }
    };

    struct datapar_transform_loop_step_ind
    {
        template <typename F, typename InIter, typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call1(F&& f, InIter& it, OutIter& dest)
        {
            typedef
                typename std::iterator_traits<InIter>::value_type value_type;

            typedef
                typename traits::detail::vector_pack_type<value_type, 1>::type
                    V1;

            invoke_vectorized_inout1_ind<V1>::call_unaligned(
                PIKA_FORWARD(F, f), it, dest);
        }

        template <typename F, typename InIter1, typename InIter2,
            typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        call1(F&& f, InIter1& it1, InIter2& it2, OutIter& dest)
        {
            typedef
                typename std::iterator_traits<InIter1>::value_type value_type1;
            typedef
                typename std::iterator_traits<InIter2>::value_type value_type2;

            typedef
                typename traits::detail::vector_pack_type<value_type1, 1>::type
                    V1;
            typedef
                typename traits::detail::vector_pack_type<value_type2, 1>::type
                    V2;

            invoke_vectorized_inout2_ind<V1, V2>::call_unaligned(
                PIKA_FORWARD(F, f), it1, it2, dest);
        }

        ///////////////////////////////////////////////////////////////////
        template <typename F, typename InIter, typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        callv(F&& f, InIter& it, OutIter& dest)
        {
            typedef
                typename std::iterator_traits<InIter>::value_type value_type;

            using V =
                typename traits::detail::vector_pack_type<value_type>::type;

            PIKA_ASSERT(is_data_aligned(it) && is_data_aligned(dest));
            invoke_vectorized_inout1_ind<V>::call_aligned(
                PIKA_FORWARD(F, f), it, dest);
        }

        template <typename F, typename InIter1, typename InIter2,
            typename OutIter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE static void
        callv(F&& f, InIter1& it1, InIter2& it2, OutIter& dest)
        {
            typedef
                typename std::iterator_traits<InIter1>::value_type value1_type;
            typedef
                typename std::iterator_traits<InIter2>::value_type value2_type;

            using V1 =
                typename traits::detail::vector_pack_type<value1_type>::type;
            using V2 =
                typename traits::detail::vector_pack_type<value2_type>::type;

            PIKA_ASSERT(is_data_aligned(it1) && is_data_aligned(it2) &&
                is_data_aligned(dest));
            invoke_vectorized_inout2_ind<V1, V2>::call_aligned(
                PIKA_FORWARD(F, f), it1, it2, dest);
        }
    };
}    // namespace pika::parallel::detail

#endif
