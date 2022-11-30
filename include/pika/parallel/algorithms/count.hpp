//  Copyright (c) 2014 Grant Mercer
//  Copyright (c) 2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/count.hpp

#pragma once

#if defined(DOXYGEN)
namespace pika {
    // clang-format off

    /// Returns the number of elements in the range [first, last) satisfying
    /// a specific criteria. This version counts the elements that are equal to
    /// the given \a value.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first comparisons.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the comparisons.
    /// \tparam FwdIterB    The type of the source begin iterator used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam FwdIterE    The type of the source end iterator used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam T           The type of the value to search for (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param value        The value to search for.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The comparisons in the parallel \a count algorithm invoked with
    /// an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// \note The comparisons in the parallel \a count algorithm invoked with
    ///       an execution policy object of type \a parallel_policy or
    ///       \a parallel_task_policy are permitted to execute in an unordered
    ///       fashion in unspecified threads, and indeterminately sequenced
    ///       within each thread.
    ///
    /// \returns  The \a count algorithm returns a
    ///           \a pika::future<difference_type> if the execution policy is of
    ///           type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a difference_type otherwise (where \a difference_type
    ///           is defined by \a std::iterator_traits<FwdIterB>::difference_type.
    ///           The \a count algorithm returns the number of elements
    ///           satisfying the given criteria.
    ///
    template <typename ExPolicy, typename FwdIterB, typename FwdIterE,
        typename T, typename Proj = pika::parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        typename std::iterator_traits<FwdIterB>::difference_type>::type
    count(ExPolicy&& policy, FwdIterB first, FwdIterE last, T const& value,
            Proj&& proj = Proj());

    /// Returns the number of elements in the range [first, last) satisfying
    /// a specific criteria. This version counts elements for which predicate
    /// \a f returns true.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first applications of
    ///         the predicate.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the comparisons.
    /// \tparam Iter        The type of the source begin iterator used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Sent        The type of the source end iterator used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam F           The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a count_if requires \a F to meet the
    ///                     requirements of \a CopyConstructible.
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param f            Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by [first, last).This is an
    ///                     unary predicate which returns \a true for the
    ///                     required elements. The signature of this predicate
    ///                     should be equivalent to:
    ///                     \code
    ///                     bool pred(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type must be such that an object of
    ///                     type \a FwdIterB can be dereferenced and then
    ///                     implicitly converted to Type.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// \note The assignments in the parallel \a count_if algorithm invoked with
    ///       an execution policy object of type \a sequenced_policy
    ///       execute in sequential order in the calling thread.
    /// \note The assignments in the parallel \a count_if algorithm invoked with
    ///       an execution policy object of type \a parallel_policy or
    ///       \a parallel_task_policy are permitted to execute in an unordered
    ///       fashion in unspecified threads, and indeterminately sequenced
    ///       within each thread.
    ///
    /// \returns  The \a count_if algorithm returns
    ///           \a pika::future<difference_type> if the execution policy is of
    ///           type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a difference_type otherwise (where \a difference_type
    ///           is defined by \a std::iterator_traits<FwdIterB>::difference_type.
    ///           The \a count algorithm returns the number of elements
    ///           satisfying the given criteria.
    ///
    template <typename ExPolicy, typename Iter, typename Sent,
        typename F, typename Proj = pika::parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        typename std::iterator_traits<Iter>::difference_type>::type
    count_if(ExPolicy&& policy, Iter first, Sent last, F&& f,
        Proj&& proj = Proj());

    // clang-format on
}    // namespace pika

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/functional/bind_back.hpp>
#include <pika/functional/invoke.hpp>
#include <pika/iterator_support/range.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/pack_traversal/unwrap.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>

#include <pika/algorithms/traits/projected.hpp>
#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/algorithms/detail/dispatch.hpp>
#include <pika/parallel/algorithms/detail/distance.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/invoke_projected.hpp>
#include <pika/parallel/util/loop.hpp>
#include <pika/parallel/util/partitioner.hpp>
#include <pika/parallel/util/vector_pack_count_bits.hpp>

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

namespace pika::parallel::detail {
    ///////////////////////////////////////////////////////////////////////////
    // count
    /// \cond NOINTERNAL
    template <typename ExPolicy, typename Op, typename Proj>
    struct count_iteration
    {
        using execution_policy_type = std::decay_t<ExPolicy>;
        using proj_type = std::decay_t<Proj>;
        using op_type = std::decay_t<Op>;

        op_type op_;
        proj_type proj_;

        template <typename Op_, typename Proj_,
            typename U = typename std::enable_if<
                !std::is_same<std::decay_t<Op_>, count_iteration>::value>::type>
        PIKA_HOST_DEVICE count_iteration(Op_&& op, Proj_&& proj)
          : op_(PIKA_FORWARD(Op_, op))
          , proj_(PIKA_FORWARD(Proj_, proj))
        {
        }

#if !defined(__NVCC__) && !defined(__CUDACC__)
        count_iteration(count_iteration const&) = default;
        count_iteration(count_iteration&&) = default;
#else
        PIKA_HOST_DEVICE count_iteration(count_iteration const& rhs)
          : op_(rhs.op_)
          , proj_(rhs.proj_)
        {
        }

        PIKA_HOST_DEVICE count_iteration(count_iteration&& rhs)
          : op_(PIKA_MOVE(rhs.op_))
          , proj_(PIKA_MOVE(rhs.proj_))
        {
        }
#endif

        count_iteration& operator=(count_iteration const&) = default;
        count_iteration& operator=(count_iteration&&) = default;

        template <typename Iter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr
            typename std::iterator_traits<Iter>::difference_type
            operator()(Iter part_begin, std::size_t part_size)
        {
            typename std::iterator_traits<Iter>::difference_type ret = 0;
            loop_n<execution_policy_type>(part_begin, part_size,
                pika::util::detail::bind_back(*this, std::ref(ret)));
            return ret;
        }

        template <typename Iter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr void operator()(Iter curr,
            typename std::iterator_traits<Iter>::difference_type& ret)
        {
            ret += traits::detail::count_bits(
                PIKA_INVOKE(op_, PIKA_INVOKE(proj_, *curr)));
        }
    };

    ///////////////////////////////////////////////////////////////////////
    template <typename Value>
    struct count : public algorithm<count<Value>, Value>
    {
        using difference_type = Value;

        count()
          : count::algorithm("count")
        {
        }

        template <typename ExPolicy, typename InIterB, typename InIterE,
            typename T, typename Proj>
        static difference_type sequential(ExPolicy&& policy, InIterB first,
            InIterE last, T const& value, Proj&& proj)
        {
            auto f1 = count_iteration<ExPolicy, detail::compare_to<T>, Proj>(
                detail::compare_to<T>(value), PIKA_FORWARD(Proj, proj));

            typename std::iterator_traits<InIterB>::difference_type ret = 0;

            loop(PIKA_FORWARD(ExPolicy, policy), first, last,
                pika::util::detail::bind_back(PIKA_MOVE(f1), std::ref(ret)));

            return ret;
        }

        template <typename ExPolicy, typename IterB, typename IterE, typename T,
            typename Proj>
        static typename algorithm_result<ExPolicy, difference_type>::type
        parallel(ExPolicy&& policy, IterB first, IterE last, T const& value,
            Proj&& proj)
        {
            if (first == last)
            {
                return algorithm_result<ExPolicy, difference_type>::get(0);
            }

            auto f1 = count_iteration<ExPolicy, detail::compare_to<T>, Proj>(
                detail::compare_to<T>(value), PIKA_FORWARD(Proj, proj));

            return detail::partitioner<ExPolicy, difference_type>::call(
                PIKA_FORWARD(ExPolicy, policy), first,
                detail::distance(first, last), PIKA_MOVE(f1),
                pika::unwrapping([](std::vector<difference_type>&& results) {
                    return accumulate_n(pika::util::begin(results),
                        pika::util::size(results), difference_type(0),
                        std::plus<difference_type>());
                }));
        }
    };
    /// \endcond

    ///////////////////////////////////////////////////////////////////////////
    // count_if
    /// \cond NOINTERNAL
    template <typename Value>
    struct count_if : public algorithm<count_if<Value>, Value>
    {
        using difference_type = Value;

        count_if()
          : count_if::algorithm("count_if")
        {
        }

        template <typename ExPolicy, typename InIterB, typename InIterE,
            typename Pred, typename Proj>
        static difference_type sequential(ExPolicy&& policy, InIterB first,
            InIterE last, Pred&& op, Proj&& proj)
        {
            auto f1 = count_iteration<ExPolicy, Pred, Proj>(
                op, PIKA_FORWARD(Proj, proj));

            typename std::iterator_traits<InIterB>::difference_type ret = 0;

            loop(PIKA_FORWARD(ExPolicy, policy), first, last,
                pika::util::detail::bind_back(PIKA_MOVE(f1), std::ref(ret)));

            return ret;
        }

        template <typename ExPolicy, typename IterB, typename IterE,
            typename Pred, typename Proj>
        static typename algorithm_result<ExPolicy, difference_type>::type
        parallel(
            ExPolicy&& policy, IterB first, IterE last, Pred&& op, Proj&& proj)
        {
            if (first == last)
            {
                return algorithm_result<ExPolicy, difference_type>::get(0);
            }

            auto f1 = count_iteration<ExPolicy, Pred, Proj>(
                op, PIKA_FORWARD(Proj, proj));

            return detail::partitioner<ExPolicy, difference_type>::call(
                PIKA_FORWARD(ExPolicy, policy), first,
                detail::distance(first, last), PIKA_MOVE(f1),
                pika::unwrapping([](std::vector<difference_type>&& results) {
                    return accumulate_n(pika::util::begin(results),
                        pika::util::size(results), difference_type(0),
                        std::plus<difference_type>());
                }));
        }
    };
    /// \endcond
}    // namespace pika::parallel::detail

namespace pika {
    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::count
    inline constexpr struct count_t final
      : pika::detail::tag_parallel_algorithm<count_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename FwdIter,
            typename T = typename std::iterator_traits<FwdIter>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            typename std::iterator_traits<FwdIter>::difference_type>::type
        tag_fallback_invoke(count_t, ExPolicy&& policy, FwdIter first,
            FwdIter last, T const& value)
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter>::value),
                "Required at least forward iterator.");

            using difference_type =
                typename std::iterator_traits<FwdIter>::difference_type;

            return pika::parallel::detail::count<difference_type>().call(
                PIKA_FORWARD(ExPolicy, policy), first, last, value,
                pika::parallel::detail::projection_identity{});
        }

        // clang-format off
        template <typename InIter,
            typename T  = typename std::iterator_traits<InIter>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter>::value
            )>
        // clang-format on
        friend typename std::iterator_traits<InIter>::difference_type
        tag_fallback_invoke(count_t, InIter first, InIter last, T const& value)
        {
            static_assert((pika::traits::is_input_iterator<InIter>::value),
                "Required at least input iterator.");

            using difference_type =
                typename std::iterator_traits<InIter>::difference_type;

            return pika::parallel::detail::count<difference_type>().call(
                pika::execution::seq, first, last, value,
                pika::parallel::detail::projection_identity{});
        }
    } count{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::count_if
    inline constexpr struct count_if_t final
      : pika::detail::tag_parallel_algorithm<count_if_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename FwdIter, typename F,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter>::value &&
                pika::detail::is_invocable_v<F,
                    typename std::iterator_traits<FwdIter>::value_type
                >
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            typename std::iterator_traits<FwdIter>::difference_type>::type
        tag_fallback_invoke(
            count_if_t, ExPolicy&& policy, FwdIter first, FwdIter last, F&& f)
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter>::value),
                "Required at least forward iterator.");

            using difference_type =
                typename std::iterator_traits<FwdIter>::difference_type;

            return pika::parallel::detail::count_if<difference_type>().call(
                PIKA_FORWARD(ExPolicy, policy), first, last, PIKA_FORWARD(F, f),
                pika::parallel::detail::projection_identity{});
        }

        // clang-format off
        template <typename InIter, typename F,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter>::value &&
                pika::detail::is_invocable_v<F,
                    typename std::iterator_traits<InIter>::value_type
                >
            )>
        // clang-format on
        friend typename std::iterator_traits<InIter>::difference_type
        tag_fallback_invoke(count_if_t, InIter first, InIter last, F&& f)
        {
            static_assert((pika::traits::is_input_iterator<InIter>::value),
                "Required at least input iterator.");

            using difference_type =
                typename std::iterator_traits<InIter>::difference_type;

            return pika::parallel::detail::count_if<difference_type>().call(
                pika::execution::seq, first, last, PIKA_FORWARD(F, f),
                pika::parallel::detail::projection_identity{});
        }
    } count_if{};
}    // namespace pika

#endif    // DOXYGEN
