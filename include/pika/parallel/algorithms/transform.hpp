//  Copyright (c) 2007-2021 Hartmut Kaiser
//  Copyright (c) 2021 Giannis Gonidelis
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/transform.hpp

#pragma once

#if defined(DOXYGEN)
namespace pika {

    /// Applies the given function \a f to the range [first, last) and stores
    /// the result in another range, beginning at dest.
    ///
    /// \note   Complexity: Exactly \a last - \a first applications of \a f
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the invocations of \a f.
    /// \tparam FwdIter1    The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam FwdIter2    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam F           The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a transform requires \a F to meet the
    ///                     requirements of \a CopyConstructible.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param f            Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by [first, last).This is an
    ///                     unary predicate. The signature of this predicate
    ///                     should be equivalent to:
    ///                     \code
    ///                     Ret fun(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&.
    ///                     The type \a Type must be such that an object of
    ///                     type \a FwdIter1 can be dereferenced and then
    ///                     implicitly converted to \a Type. The type \a Ret
    ///                     must be such that an object of type \a FwdIter2 can
    ///                     be dereferenced and assigned a value of type
    ///                     \a Ret.
    ///
    /// The invocations of \a f in the parallel \a transform algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The invocations of \a f in the parallel \a transform algorithm invoked
    /// with an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a transform algorithm returns a
    /// \a pika::future<in_out_result<FwdIter1, FwdIter2> >
    ///           if the execution policy is of type \a parallel_task_policy
    ///           and returns
    /// \a in_out_result<FwdIter1, FwdIter2> otherwise.
    ///           The \a transform algorithm returns a tuple holding an iterator
    ///           referring to the first element after the input sequence and
    ///           the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
        typename F>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        detail::in_out_result<FwdIter1, FwdIter2>>::type
    transform(
        ExPolicy&& policy, FwdIter1 first, FwdIter1 last, FwdIter2 dest, F&& f);

    /// Applies the given function \a f to pairs of elements from two ranges:
    /// one defined by [first1, last1) and the other beginning at first2, and
    /// stores the result in another range, beginning at dest.
    ///
    /// \note   Complexity: Exactly \a last - \a first applications of \a f
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the invocations of \a f.
    /// \tparam FwdIter1    The type of the source iterators for the first
    ///                     range used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam FwdIter2    The type of the source iterators for the second
    ///                     range used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam FwdIter3    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam F           The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a transform requires \a F to meet the
    ///                     requirements of \a CopyConstructible.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first1       Refers to the beginning of the first sequence of
    ///                     elements the algorithm will be applied to.
    /// \param last1        Refers to the end of the first sequence of elements
    ///                     the algorithm will be applied to.
    /// \param first2       Refers to the beginning of the second sequence of
    ///                     elements the algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param f            Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by [first, last).This is a
    ///                     binary predicate. The signature of this predicate
    ///                     should be equivalent to:
    ///                     \code
    ///                     Ret fun(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const&.
    ///                     The types \a Type1 and \a Type2 must be such that
    ///                     objects of types FwdIter1 and FwdIter2 can be
    ///                     dereferenced and then implicitly converted to
    ///                     \a Type1 and \a Type2 respectively. The type \a Ret
    ///                     must be such that an object of type \a FwdIter3 can
    ///                     be dereferenced and assigned a value of type
    ///                     \a Ret.
    ///
    /// The invocations of \a f in the parallel \a transform algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The invocations of \a f in the parallel \a transform algorithm invoked
    /// with an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a transform algorithm returns a
    /// \a pika::future<in_in_out_result<FwdIter1, FwdIter2, FwdIter3> >
    ///           if the execution policy is of type \a parallel_task_policy
    ///           and returns
    /// \a in_in_out_result<FwdIter1, FwdIter2, FwdIter3>
    ///           otherwise.
    ///           The \a transform algorithm returns a tuple holding an iterator
    ///           referring to the first element after the first input sequence,
    ///           an iterator referring to the first element after the second
    ///           input sequence, and the output iterator referring to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
        typename FwdIter3, typename F>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        detail::in_in_out_result<FwdIter1, FwdIter2, FwdIter3>>::type
    transform(ExPolicy&& policy, FwdIter1 first1, FwdIter1 last1,
        FwdIter2 first2, FwdIter3 dest, F&& f);

}    // namespace pika

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#if defined(PIKA_HAVE_THREAD_DESCRIPTION)
#include <pika/functional/traits/get_function_address.hpp>
#include <pika/functional/traits/get_function_annotation.hpp>
#endif
#include <pika/functional/invoke.hpp>
#include <pika/functional/traits/is_invocable.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>
#include <pika/parallel/util/result_types.hpp>

#include <pika/algorithms/traits/projected.hpp>
#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/algorithms/detail/dispatch.hpp>
#include <pika/parallel/algorithms/detail/distance.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/foreach_partitioner.hpp>
#include <pika/parallel/util/projection_identity.hpp>
#include <pika/parallel/util/transform_loop.hpp>
#include <pika/parallel/util/zip_iterator.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace pika::parallel::detail {
    ///////////////////////////////////////////////////////////////////////////
    // transform
    /// \cond NOINTERNAL
    template <typename F, typename Proj>
    struct transform_projected
    {
        std::decay_t<F>& f_;
        std::decay_t<Proj>& proj_;

        PIKA_HOST_DEVICE constexpr transform_projected(
            F& f, Proj& proj) noexcept
          : f_(f)
          , proj_(proj)
        {
        }

        template <typename Iter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr auto operator()(Iter curr)
            -> decltype(PIKA_INVOKE(f_, PIKA_INVOKE(proj_, *curr)))
        {
            return PIKA_INVOKE(f_, PIKA_INVOKE(proj_, *curr));
        }
    };

    template <typename F>
    struct transform_projected<F, projection_identity>
    {
        std::decay_t<F>& f_;

        PIKA_HOST_DEVICE constexpr transform_projected(
            F& f, projection_identity) noexcept
          : f_(f)
        {
        }

        template <typename Iter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr auto operator()(Iter curr)
            -> decltype(PIKA_INVOKE(f_, *curr))
        {
            return PIKA_INVOKE(f_, *curr);
        }
    };

    template <typename ExPolicy, typename F, typename Proj>
    struct transform_iteration
    {
        using execution_policy_type = std::decay_t<ExPolicy>;
        using fun_type = std::decay_t<F>;
        using proj_type = std::decay_t<Proj>;

        fun_type f_;
        proj_type proj_;

        template <typename F_, typename Proj_>
        PIKA_HOST_DEVICE transform_iteration(F_&& f, Proj_&& proj)
          : f_(PIKA_FORWARD(F_, f))
          , proj_(PIKA_FORWARD(Proj_, proj))
        {
        }

#if !defined(__NVCC__) && !defined(__CUDACC__)
        transform_iteration(transform_iteration const&) = default;
        transform_iteration(transform_iteration&&) = default;
#else
        PIKA_HOST_DEVICE transform_iteration(transform_iteration const& rhs)
          : f_(rhs.f_)
          , proj_(rhs.proj_)
        {
        }

        PIKA_HOST_DEVICE transform_iteration(transform_iteration&& rhs)
          : f_(PIKA_MOVE(rhs.f_))
          , proj_(PIKA_MOVE(rhs.proj_))
        {
        }
#endif
        transform_iteration& operator=(transform_iteration const&) = default;
        transform_iteration& operator=(transform_iteration&&) = default;

        template <typename Iter, typename F_ = fun_type>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE
            std::pair<typename std::tuple_element<0,
                          typename Iter::iterator_tuple_type>::type,
                typename std::tuple_element<1,
                    typename Iter::iterator_tuple_type>::type>
            operator()(Iter part_begin, std::size_t part_size, std::size_t)
        {
            auto iters = part_begin.get_iterator_tuple();
            return transform_loop_n<execution_policy_type>(std::get<0>(iters),
                part_size, std::get<1>(iters),
                transform_projected<F, Proj>(f_, proj_));
        }
    };

    template <typename ExPolicy, typename F>
    struct transform_iteration<ExPolicy, F, projection_identity>
    {
        using execution_policy_type = std::decay_t<ExPolicy>;
        using fun_type = std::decay_t<F>;

        fun_type f_;

        template <typename F_>
        PIKA_HOST_DEVICE transform_iteration(F_&& f, projection_identity)
          : f_(PIKA_FORWARD(F_, f))
        {
        }

#if !defined(__NVCC__) && !defined(__CUDACC__)
        transform_iteration(transform_iteration const&) = default;
        transform_iteration(transform_iteration&&) = default;
#else
        PIKA_HOST_DEVICE transform_iteration(transform_iteration const& rhs)
          : f_(rhs.f_)
        {
        }

        PIKA_HOST_DEVICE transform_iteration(transform_iteration&& rhs)
          : f_(PIKA_MOVE(rhs.f_))
        {
        }
#endif
        transform_iteration& operator=(transform_iteration const&) = default;
        transform_iteration& operator=(transform_iteration&&) = default;

        template <typename Iter, typename F_ = fun_type>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE
            std::pair<typename std::tuple_element<0,
                          typename Iter::iterator_tuple_type>::type,
                typename std::tuple_element<1,
                    typename Iter::iterator_tuple_type>::type>
            operator()(Iter part_begin, std::size_t part_size, std::size_t)
        {
            auto iters = part_begin.get_iterator_tuple();
            return transform_loop_n_ind<execution_policy_type>(
                std::get<0>(iters), part_size, std::get<1>(iters), f_);
        }
    };

    ///////////////////////////////////////////////////////////////////////
    template <typename IterPair>
    struct transform : public algorithm<transform<IterPair>, IterPair>
    {
        transform()
          : transform::algorithm("transform")
        {
        }

        // sequential execution with non-trivial projection
        template <typename ExPolicy, typename InIterB, typename InIterE,
            typename OutIter, typename F, typename Proj>
        PIKA_HOST_DEVICE static in_out_result<InIterB, OutIter>
        sequential(ExPolicy&& policy, InIterB first, InIterE last, OutIter dest,
            F&& f, Proj&& proj)
        {
            return transform_loop(PIKA_FORWARD(ExPolicy, policy), first, last,
                dest, transform_projected<F, Proj>(f, proj));
        }

        // sequential execution without projection
        template <typename ExPolicy, typename InIterB, typename InIterE,
            typename OutIter, typename F>
        PIKA_HOST_DEVICE static in_out_result<InIterB, OutIter>
        sequential(ExPolicy&& policy, InIterB first, InIterE last, OutIter dest,
            F&& f, projection_identity)
        {
            return transform_loop_ind(PIKA_FORWARD(ExPolicy, policy), first,
                last, dest, PIKA_FORWARD(F, f));
        }

        template <typename ExPolicy, typename FwdIter1B, typename FwdIter1E,
            typename FwdIter2, typename F, typename Proj>
        static typename algorithm_result<ExPolicy,
            in_out_result<FwdIter1B, FwdIter2>>::type
        parallel(ExPolicy&& policy, FwdIter1B first, FwdIter1E last,
            FwdIter2 dest, F&& f, Proj&& proj)
        {
            if (first != last)
            {
                auto f1 = transform_iteration<ExPolicy, F, Proj>(
                    PIKA_FORWARD(F, f), PIKA_FORWARD(Proj, proj));

                return get_in_out_result(foreach_partitioner<ExPolicy>::call(
                    PIKA_FORWARD(ExPolicy, policy),
                    pika::util::make_zip_iterator(first, dest),
                    detail::distance(first, last), PIKA_MOVE(f1),
                    projection_identity()));
            }

            using result_type = in_out_result<FwdIter1B, FwdIter2>;

            return algorithm_result<ExPolicy, result_type>::get(
                result_type{PIKA_MOVE(first), PIKA_MOVE(dest)});
        }
    };
    /// \endcond

    ///////////////////////////////////////////////////////////////////////////
    // transform binary predicate
    /// \cond NOINTERNAL
    template <typename F, typename Proj1, typename Proj2>
    struct transform_binary_projected
    {
        std::decay_t<F>& f_;
        std::decay_t<Proj1>& proj1_;
        std::decay_t<Proj2>& proj2_;

        template <typename Iter1, typename Iter2>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE auto
        operator()(Iter1 curr1, Iter2 curr2)
        {
            return PIKA_INVOKE(
                f_, PIKA_INVOKE(proj1_, *curr1), PIKA_INVOKE(proj2_, *curr2));
        }
    };

    template <typename ExPolicy, typename F, typename Proj1, typename Proj2>
    struct transform_binary_iteration
    {
        using execution_policy_type = std::decay_t<ExPolicy>;
        using fun_type = std::decay_t<F>;
        using proj1_type = std::decay_t<Proj1>;
        using proj2_type = std::decay_t<Proj2>;

        fun_type f_;
        proj1_type proj1_;
        proj2_type proj2_;

        template <typename F_, typename Proj1_, typename Proj2_>
        PIKA_HOST_DEVICE
        transform_binary_iteration(F_&& f, Proj1_&& proj1, Proj2_&& proj2)
          : f_(PIKA_FORWARD(F_, f))
          , proj1_(PIKA_FORWARD(Proj1_, proj1))
          , proj2_(PIKA_FORWARD(Proj2_, proj2))
        {
        }

#if !defined(__NVCC__) && !defined(__CUDACC__)
        transform_binary_iteration(transform_binary_iteration const&) = default;
        transform_binary_iteration(transform_binary_iteration&&) = default;
#else
        PIKA_HOST_DEVICE
        transform_binary_iteration(transform_binary_iteration const& rhs)
          : f_(rhs.f_)
          , proj1_(rhs.proj1_)
          , proj2_(rhs.proj2_)
        {
        }

        PIKA_HOST_DEVICE
        transform_binary_iteration(transform_binary_iteration&& rhs)
          : f_(PIKA_MOVE(rhs.f_))
          , proj1_(PIKA_MOVE(rhs.proj1_))
          , proj2_(PIKA_MOVE(rhs.proj2_))
        {
        }
#endif
        transform_binary_iteration& operator=(
            transform_binary_iteration const&) = default;
        transform_binary_iteration& operator=(
            transform_binary_iteration&&) = default;

        template <typename Iter, typename F_ = fun_type>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE
            std::tuple<typename std::tuple_element<0,
                           typename Iter::iterator_tuple_type>::type,
                typename std::tuple_element<1,
                    typename Iter::iterator_tuple_type>::type,
                typename std::tuple_element<2,
                    typename Iter::iterator_tuple_type>::type>
            operator()(Iter part_begin, std::size_t part_size, std::size_t)
        {
            auto iters = part_begin.get_iterator_tuple();
            return transform_binary_loop_n<execution_policy_type>(
                std::get<0>(iters), part_size, std::get<1>(iters),
                std::get<2>(iters),
                transform_binary_projected<F_, Proj1, Proj2>{
                    f_, proj1_, proj2_});
        }
    };

    template <typename ExPolicy, typename F>
    struct transform_binary_iteration<ExPolicy, F, projection_identity,
        projection_identity>
    {
        using execution_policy_type = std::decay_t<ExPolicy>;
        using fun_type = std::decay_t<F>;

        fun_type f_;

        template <typename F_>
        PIKA_HOST_DEVICE transform_binary_iteration(
            F_&& f, projection_identity, projection_identity)
          : f_(PIKA_FORWARD(F_, f))
        {
        }

#if !defined(__NVCC__) && !defined(__CUDACC__)
        transform_binary_iteration(transform_binary_iteration const&) = default;
        transform_binary_iteration(transform_binary_iteration&&) = default;
#else
        PIKA_HOST_DEVICE
        transform_binary_iteration(transform_binary_iteration const& rhs)
          : f_(rhs.f_)
        {
        }

        PIKA_HOST_DEVICE
        transform_binary_iteration(transform_binary_iteration&& rhs)
          : f_(PIKA_MOVE(rhs.f_))
        {
        }
#endif
        transform_binary_iteration& operator=(
            transform_binary_iteration const&) = default;
        transform_binary_iteration& operator=(
            transform_binary_iteration&&) = default;

        template <typename Iter, typename F_ = fun_type>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE
            std::tuple<typename std::tuple_element<0,
                           typename Iter::iterator_tuple_type>::type,
                typename std::tuple_element<1,
                    typename Iter::iterator_tuple_type>::type,
                typename std::tuple_element<2,
                    typename Iter::iterator_tuple_type>::type>
            operator()(Iter part_begin, std::size_t part_size, std::size_t)
        {
            auto iters = part_begin.get_iterator_tuple();
            return transform_binary_loop_ind_n<execution_policy_type>(
                std::get<0>(iters), part_size, std::get<1>(iters),
                std::get<2>(iters), f_);
        }
    };

    ///////////////////////////////////////////////////////////////////////
    template <typename IterTuple>
    struct transform_binary
      : public algorithm<transform_binary<IterTuple>, IterTuple>
    {
        transform_binary()
          : transform_binary::algorithm("transform_binary")
        {
        }

        // sequential execution with non-trivial projection
        template <typename ExPolicy, typename InIter1, typename InIter2,
            typename OutIter, typename F, typename Proj1, typename Proj2>
        static in_in_out_result<InIter1, InIter2, OutIter>
        sequential(ExPolicy&&, InIter1 first1, InIter1 last1, InIter2 first2,
            OutIter dest, F&& f, Proj1&& proj1, Proj2&& proj2)
        {
            return transform_binary_loop<ExPolicy>(first1, last1, first2, dest,
                transform_binary_projected<F, Proj1, Proj2>{f, proj1, proj2});
        }

        // sequential execution without projection
        template <typename ExPolicy, typename InIter1, typename InIter2,
            typename OutIter, typename F>
        static in_in_out_result<InIter1, InIter2, OutIter>
        sequential(ExPolicy&&, InIter1 first1, InIter1 last1, InIter2 first2,
            OutIter dest, F&& f, projection_identity, projection_identity)
        {
            return transform_binary_loop_ind<ExPolicy>(
                first1, last1, first2, dest, f);
        }

        template <typename ExPolicy, typename FwdIter1B, typename FwdIter1E,
            typename FwdIter2, typename FwdIter3, typename F, typename Proj1,
            typename Proj2>
        static typename algorithm_result<ExPolicy,
            in_in_out_result<FwdIter1B, FwdIter2, FwdIter3>>::type
        parallel(ExPolicy&& policy, FwdIter1B first1, FwdIter1E last1,
            FwdIter2 first2, FwdIter3 dest, F&& f, Proj1&& proj1, Proj2&& proj2)
        {
            if (first1 != last1)
            {
                auto f1 = transform_binary_iteration<ExPolicy, F, Proj1, Proj2>(
                    PIKA_FORWARD(F, f), PIKA_FORWARD(Proj1, proj1),
                    PIKA_FORWARD(Proj2, proj2));

                return get_in_in_out_result(foreach_partitioner<ExPolicy>::call(
                    PIKA_FORWARD(ExPolicy, policy),
                    pika::util::make_zip_iterator(first1, first2, dest),
                    detail::distance(first1, last1), PIKA_MOVE(f1),
                    projection_identity()));
            }

            using result_type = in_in_out_result<FwdIter1B, FwdIter2, FwdIter3>;

            return algorithm_result<ExPolicy, result_type>::get(result_type{
                PIKA_MOVE(first1), PIKA_MOVE(first2), PIKA_MOVE(dest)});
        }
    };
    /// \endcond

    ///////////////////////////////////////////////////////////////////////////
    // transform binary predicate
    /// \cond NOINTERNAL
    template <typename IterTuple>
    struct transform_binary2
      : public algorithm<transform_binary2<IterTuple>, IterTuple>
    {
        transform_binary2()
          : transform_binary2::algorithm("transform_binary")
        {
        }

        // sequential execution with non-trivial projection
        template <typename ExPolicy, typename InIter1, typename InIter2,
            typename OutIter, typename F, typename Proj1, typename Proj2>
        static in_in_out_result<InIter1, InIter2, OutIter>
        sequential(ExPolicy&&, InIter1 first1, InIter1 last1, InIter2 first2,
            InIter2 last2, OutIter dest, F&& f, Proj1&& proj1, Proj2&& proj2)
        {
            return transform_binary_loop<ExPolicy>(first1, last1, first2, last2,
                dest,
                transform_binary_projected<F, Proj1, Proj2>{f, proj1, proj2});
        }

        // sequential execution without projection
        template <typename ExPolicy, typename InIter1, typename InIter2,
            typename OutIter, typename F>
        static in_in_out_result<InIter1, InIter2, OutIter>
        sequential(ExPolicy&&, InIter1 first1, InIter1 last1, InIter2 first2,
            InIter2 last2, OutIter dest, F&& f, projection_identity,
            projection_identity)
        {
            return transform_binary_loop_ind<ExPolicy>(
                first1, last1, first2, last2, dest, f);
        }

        template <typename ExPolicy, typename FwdIter1B, typename FwdIter1E,
            typename FwdIter2B, typename FwdIter2E, typename FwdIter3,
            typename F, typename Proj1, typename Proj2>
        static typename algorithm_result<ExPolicy,
            in_in_out_result<FwdIter1B, FwdIter2B, FwdIter3>>::type
        parallel(ExPolicy&& policy, FwdIter1B first1, FwdIter1E last1,
            FwdIter2B first2, FwdIter2E last2, FwdIter3 dest, F&& f,
            Proj1&& proj1, Proj2&& proj2)
        {
            if (first1 != last1 && first2 != last2)
            {
                auto f1 = transform_binary_iteration<ExPolicy, F, Proj1, Proj2>(
                    PIKA_FORWARD(F, f), PIKA_FORWARD(Proj1, proj1),
                    PIKA_FORWARD(Proj2, proj2));

                // different versions of clang-format do different things
                // clang-format off
                    return get_in_in_out_result(
                        foreach_partitioner<ExPolicy>::call(
                            PIKA_FORWARD(ExPolicy, policy),
                            pika::util::make_zip_iterator(first1, first2, dest),
                            (std::min) (detail::distance(first1, last1),
                                detail::distance(first2, last2)),
                            PIKA_MOVE(f1), projection_identity()));
                // clang-format on
            }

            using result_type =
                in_in_out_result<FwdIter1B, FwdIter2B, FwdIter3>;

            return algorithm_result<ExPolicy, result_type>::get(result_type{
                PIKA_MOVE(first1), PIKA_MOVE(first2), PIKA_MOVE(dest)});
        }
    };
    /// \endcond
}    // namespace pika::parallel::detail

#if defined(PIKA_HAVE_THREAD_DESCRIPTION)
namespace pika::detail {
    template <typename ExPolicy, typename F, typename Proj>
    struct get_function_address<
        parallel::detail::transform_iteration<ExPolicy, F, Proj>>
    {
        static constexpr std::size_t call(
            parallel::detail::transform_iteration<ExPolicy, F, Proj> const&
                f) noexcept
        {
            return get_function_address<std::decay_t<F>>::call(f.f_);
        }
    };

    template <typename ExPolicy, typename F, typename Proj>
    struct get_function_annotation<
        parallel::detail::transform_iteration<ExPolicy, F, Proj>>
    {
        static constexpr char const* call(
            parallel::detail::transform_iteration<ExPolicy, F, Proj> const&
                f) noexcept
        {
            return get_function_annotation<std::decay_t<F>>::call(f.f_);
        }
    };

    template <typename ExPolicy, typename F, typename Proj1, typename Proj2>
    struct get_function_address<
        parallel::detail::transform_binary_iteration<ExPolicy, F, Proj1, Proj2>>
    {
        static constexpr std::size_t call(
            parallel::detail::transform_binary_iteration<ExPolicy, F, Proj1,
                Proj2> const& f) noexcept
        {
            return get_function_address<std::decay_t<F>>::call(f.f_);
        }
    };

    template <typename ExPolicy, typename F, typename Proj1, typename Proj2>
    struct get_function_annotation<
        parallel::detail::transform_binary_iteration<ExPolicy, F, Proj1, Proj2>>
    {
        static constexpr char const* call(
            parallel::detail::transform_binary_iteration<ExPolicy, F, Proj1,
                Proj2> const& f) noexcept
        {
            return get_function_annotation<std::decay_t<F>>::call(f.f_);
        }
    };

#if PIKA_HAVE_ITTNOTIFY != 0 && !defined(PIKA_HAVE_APEX)
    template <typename ExPolicy, typename F, typename Proj>
    struct get_function_annotation_itt<
        parallel::detail::transform_iteration<ExPolicy, F, Proj>>
    {
        static util::itt::string_handle call(
            parallel::detail::transform_iteration<ExPolicy, F, Proj> const&
                f) noexcept
        {
            return get_function_annotation_itt<std::decay_t<F>>::call(f.f_);
        }
    };

    template <typename ExPolicy, typename F, typename Proj1, typename Proj2>
    struct get_function_annotation_itt<
        parallel::detail::transform_binary_iteration<ExPolicy, F, Proj1, Proj2>>
    {
        static util::itt::string_handle call(
            parallel::detail::transform_binary_iteration<ExPolicy, F, Proj1,
                Proj2> const& f) noexcept
        {
            return get_function_annotation_itt<std::decay_t<F>>::call(f.f_);
        }
    };
#endif
}    // namespace pika::detail
#endif

namespace pika {
    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::transform
    inline constexpr struct transform_t final
      : pika::detail::tag_parallel_algorithm<transform_t>
    {
    private:
        // clang-format off
        template <typename FwdIter1, typename FwdIter2,
            typename F,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator_v<FwdIter1> &&
                pika::traits::is_iterator_v<FwdIter2>
            )>
        // clang-format on
        friend FwdIter2 tag_fallback_invoke(pika::transform_t, FwdIter1 first,
            FwdIter1 last, FwdIter2 dest, F&& f)
        {
            static_assert(pika::traits::is_input_iterator<FwdIter1>::value,
                "Requires at least input iterator.");

            return pika::parallel::detail::get_second_element(
                parallel::detail::transform<
                    pika::parallel::detail::in_out_result<FwdIter1, FwdIter2>>()
                    .call(pika::execution::seq, first, last, dest,
                        PIKA_FORWARD(F, f),
                        pika::parallel::detail::projection_identity{}));
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
            typename F,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy_v<ExPolicy> &&
                pika::traits::is_iterator_v<FwdIter1> &&
                pika::traits::is_iterator_v<FwdIter2>
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            FwdIter2>::type
        tag_fallback_invoke(pika::transform_t, ExPolicy&& policy,
            FwdIter1 first, FwdIter1 last, FwdIter2 dest, F&& f)
        {
            static_assert(pika::traits::is_forward_iterator<FwdIter1>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::get_second_element(
                parallel::detail::transform<
                    pika::parallel::detail::in_out_result<FwdIter1, FwdIter2>>()
                    .call(PIKA_FORWARD(ExPolicy, policy), first, last, dest,
                        PIKA_FORWARD(F, f),
                        pika::parallel::detail::projection_identity{}));
        }

        // clang-format off
        template <typename FwdIter1, typename FwdIter2, typename FwdIter3,
            typename F,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator_v<FwdIter1> &&
                pika::traits::is_iterator_v<FwdIter2> &&
                pika::traits::is_iterator_v<FwdIter3>
            )>
        // clang-format on
        friend FwdIter3 tag_fallback_invoke(pika::transform_t, FwdIter1 first1,
            FwdIter1 last1, FwdIter2 first2, FwdIter3 dest, F&& f)
        {
            static_assert(pika::traits::is_input_iterator<FwdIter1>::value &&
                    pika::traits::is_input_iterator<FwdIter2>::value,
                "Requires at least input iterator.");

            using proj_id = pika::parallel::detail::projection_identity;
            using result_type =
                pika::parallel::detail::in_in_out_result<FwdIter1, FwdIter2,
                    FwdIter3>;

            return parallel::detail::get_third_element(
                parallel::detail::transform_binary<result_type>().call(
                    pika::execution::seq, first1, last1, first2, dest,
                    PIKA_FORWARD(F, f), proj_id(), proj_id()));
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter1,
            typename FwdIter2, typename FwdIter3,
            typename F,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy_v<ExPolicy> &&
                pika::traits::is_iterator_v<FwdIter1> &&
                pika::traits::is_iterator_v<FwdIter2> &&
                pika::traits::is_iterator_v<FwdIter3>
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            FwdIter3>::type
        tag_fallback_invoke(pika::transform_t, ExPolicy&& policy,
            FwdIter1 first1, FwdIter1 last1, FwdIter2 first2, FwdIter3 dest,
            F&& f)
        {
            static_assert(pika::traits::is_input_iterator<FwdIter1>::value &&
                    pika::traits::is_input_iterator<FwdIter2>::value,
                "Requires at least input iterator.");

            using proj_id = pika::parallel::detail::projection_identity;
            using result_type =
                pika::parallel::detail::in_in_out_result<FwdIter1, FwdIter2,
                    FwdIter3>;

            return parallel::detail::get_third_element(
                parallel::detail::transform_binary<result_type>().call(
                    PIKA_FORWARD(ExPolicy, policy), first1, last1, first2, dest,
                    PIKA_FORWARD(F, f), proj_id(), proj_id()));
        }

    } transform{};
}    // namespace pika

#endif    // DOXYGEN
