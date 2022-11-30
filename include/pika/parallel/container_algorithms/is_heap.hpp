//  Copyright (c) 2017 Taeguk Kwon
//  Copyright (c) 2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/container_algorithms/is_heap.hpp

#pragma once

#if defined(DOXYGEN)
namespace pika {
    // clang-format off

    /// Returns whether the range is max heap. That is, true if the range is
    /// max heap, false otherwise. The function uses the given comparison
    /// function object \a comp (defaults to using operator<()).
    ///
    /// \note   Complexity:
    ///         Performs at most N applications of the comparison \a comp,
    ///         at most 2 * N applications of the projection \a proj,
    ///         where N = last - first.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter        The type of the begin source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Sent        The type of the end source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     sentinel for Iter1.
    /// \tparam Comp        The type of the function/function object to use
    ///                     (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param iter         Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param sent         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param comp         \a comp is a callable object. The return value of the
    ///                     INVOKE operation applied to an object of type \a Comp,
    ///                     when contextually converted to bool, yields true if
    ///                     the first argument of the call is less than the
    ///                     second, and false otherwise. It is assumed that comp
    ///                     will not apply any non-constant function through the
    ///                     dereferenced iterator.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// \a comp has to induce a strict weak ordering on the values.
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a sequenced_policy execute in sequential order in the
    /// calling thread.
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a parallel_policy or \a parallel_task_policy are
    /// permitted to execute in an unordered fashion in unspecified
    /// threads, and indeterminately sequenced within each thread.
    ///
    /// \returns  The \a is_heap algorithm returns a \a pika::future<bool>
    ///           if the execution policy is of type \a sequenced_task_policy or
    ///           \a parallel_task_policy and returns \a bool otherwise.
    ///           The \a is_heap algorithm returns whether the range is max heap.
    ///           That is, true if the range is max heap, false otherwise.
    ///
    template <typename ExPolicy, typename Iter, typename Sent,
        typename Comp = detail::less,
        typename Proj = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy, bool>::type
    is_heap(ExPolicy&& policy, Iter first, Sent last, Comp&& comp = Comp(),
        Proj&& proj = Proj());

    /// Returns whether the range is max heap. That is, true if the range is
    /// max heap, false otherwise. The function uses the given comparison
    /// function object \a comp (defaults to using operator<()).
    ///
    /// \note   Complexity:
    ///         Performs at most N applications of the comparison \a comp,
    ///         at most 2 * N applications of the projection \a proj,
    ///         where N = last - first.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an random access iterator.
    /// \tparam Comp        The type of the function/function object to use
    ///                     (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param comp         \a comp is a callable object. The return value of the
    ///                     INVOKE operation applied to an object of type \a Comp,
    ///                     when contextually converted to bool, yields true if
    ///                     the first argument of the call is less than the
    ///                     second, and false otherwise. It is assumed that comp
    ///                     will not apply any non-constant function through the
    ///                     dereferenced iterator.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// \a comp has to induce a strict weak ordering on the values.
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a sequenced_policy execute in sequential order in the
    /// calling thread.
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a parallel_policy or \a parallel_task_policy are
    /// permitted to execute in an unordered fashion in unspecified
    /// threads, and indeterminately sequenced within each thread.
    ///
    /// \returns  The \a is_heap algorithm returns a \a pika::future<bool>
    ///           if the execution policy is of type \a sequenced_task_policy or
    ///           \a parallel_task_policy and returns \a bool otherwise.
    ///           The \a is_heap algorithm returns whether the range is max heap.
    ///           That is, true if the range is max heap, false otherwise.
    ///
    template <typename ExPolicy, typename Rng, typename Comp = detail::less,
        typename Proj = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy, bool>::type
    is_heap(ExPolicy&& policy, Rng&& rng, Comp&& comp = Comp(),
        Proj&& proj = Proj());

    /// Returns the upper bound of the largest range beginning at \a first
    /// which is a max heap. That is, the last iterator \a it for
    /// which range [first, it) is a max heap. The function
    /// uses the given comparison function object \a comp (defaults to using
    /// operator<()).
    ///
    /// \note   Complexity:
    ///         Performs at most N applications of the comparison \a comp,
    ///         at most 2 * N applications of the projection \a proj,
    ///         where N = last - first.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter        The type of the begin source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Sent        The type of the end source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     sentinel for Iter1.
    /// \tparam Comp        The type of the function/function object to use
    ///                     (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param iter         Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param sent         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param comp         \a comp is a callable object. The return value of the
    ///                     INVOKE operation applied to an object of type \a Comp,
    ///                     when contextually converted to bool, yields true if
    ///                     the first argument of the call is less than the
    ///                     second, and false otherwise. It is assumed that comp
    ///                     will not apply any non-constant function through the
    ///                     dereferenced iterator.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// \a comp has to induce a strict weak ordering on the values.
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a sequenced_policy execute in sequential order in the
    /// calling thread.
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a parallel_policy or \a parallel_task_policy are
    /// permitted to execute in an unordered fashion in unspecified
    /// threads, and indeterminately sequenced within each thread.
    ///
    /// \returns  The \a is_heap_until algorithm returns a \a pika::future<RandIter>
    ///           if the execution policy is of type \a sequenced_task_policy or
    ///           \a parallel_task_policy and returns \a RandIter otherwise.
    ///           The \a is_heap_until algorithm returns the upper bound
    ///           of the largest range beginning at first which is a max heap.
    ///           That is, the last iterator \a it for which range [first, it)
    ///           is a max heap.
    ///
    template <typename ExPolicy, typename Iter, typename Sent,
        typename Comp = detail::less,
        typename Proj = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy, Iter>::type
    is_heap_until(ExPolicy&& policy, Iter first, Sent sent, Comp&& comp = Comp(),
        Proj&& proj = Proj());

    /// Returns the upper bound of the largest range beginning at \a first
    /// which is a max heap. That is, the last iterator \a it for
    /// which range [first, it) is a max heap. The function
    /// uses the given comparison function object \a comp (defaults to using
    /// operator<()).
    ///
    /// \note   Complexity:
    ///         Performs at most N applications of the comparison \a comp,
    ///         at most 2 * N applications of the projection \a proj,
    ///         where N = last - first.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an random access iterator.
    /// \tparam Comp        The type of the function/function object to use
    ///                     (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param comp         \a comp is a callable object. The return value of the
    ///                     INVOKE operation applied to an object of type \a Comp,
    ///                     when contextually converted to bool, yields true if
    ///                     the first argument of the call is less than the
    ///                     second, and false otherwise. It is assumed that comp
    ///                     will not apply any non-constant function through the
    ///                     dereferenced iterator.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// \a comp has to induce a strict weak ordering on the values.
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a sequenced_policy execute in sequential order in the
    /// calling thread.
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a parallel_policy or \a parallel_task_policy are
    /// permitted to execute in an unordered fashion in unspecified
    /// threads, and indeterminately sequenced within each thread.
    ///
    /// \returns  The \a is_heap_until algorithm returns a \a pika::future<RandIter>
    ///           if the execution policy is of type \a sequenced_task_policy or
    ///           \a parallel_task_policy and returns \a RandIter otherwise.
    ///           The \a is_heap_until algorithm returns the upper bound
    ///           of the largest range beginning at first which is a max heap.
    ///           That is, the last iterator \a it for which range [first, it)
    ///           is a max heap.
    ///
    template <typename ExPolicy, typename Rng, typename Comp = detail::less,
        typename Proj = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        typename pika::traits::range_iterator<Rng>::type>::type
    is_heap_until(ExPolicy&& policy, Rng&& rng, Comp&& comp = Comp(),
        Proj&& proj = Proj());

    // clang-format on
}    // namespace pika

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/iterator_support/range.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/iterator_support/traits/is_range.hpp>

#include <pika/algorithms/traits/projected.hpp>
#include <pika/algorithms/traits/projected_range.hpp>
#include <pika/parallel/algorithms/is_heap.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>
#include <pika/parallel/util/projection_identity.hpp>

#include <type_traits>
#include <utility>

namespace pika::ranges {
    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::is_heap
    inline constexpr struct is_heap_t final
      : pika::detail::tag_parallel_algorithm<is_heap_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename Rng,
            typename Comp = pika::parallel::detail::less,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy, Comp,
                    pika::parallel::detail::projected_range<Proj, Rng>,
                    pika::parallel::detail::projected_range<Proj, Rng>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            bool>::type
        tag_fallback_invoke(is_heap_t, ExPolicy&& policy, Rng&& rng,
            Comp&& comp = Comp(), Proj&& proj = Proj())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng>::type;

            static_assert(
                (pika::traits::is_random_access_iterator<iterator_type>::value),
                "Requires a random access iterator.");

            return pika::parallel::detail::is_heap<iterator_type>().call(
                PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng),
                pika::util::end(rng), PIKA_FORWARD(Comp, comp),
                PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Iter, typename Sent,
            typename Comp = pika::parallel::detail::less,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy, Comp,
                    pika::parallel::detail::projected<Proj, Iter>,
                    pika::parallel::detail::projected<Proj, Iter>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            bool>::type
        tag_fallback_invoke(is_heap_t, ExPolicy&& policy, Iter first, Sent last,
            Comp&& comp = Comp(), Proj&& proj = Proj())
        {
            static_assert(
                (pika::traits::is_random_access_iterator<Iter>::value),
                "Requires a random access iterator.");

            return pika::parallel::detail::is_heap<Iter>().call(
                PIKA_FORWARD(ExPolicy, policy), first, last,
                PIKA_FORWARD(Comp, comp), PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Rng,
            typename Comp = pika::parallel::detail::less,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::parallel::detail::is_indirect_callable<
                    pika::execution::sequenced_policy, Comp,
                    pika::parallel::detail::projected_range<Proj, Rng>,
                    pika::parallel::detail::projected_range<Proj, Rng>
                >::value
            )>
        // clang-format on
        friend bool tag_fallback_invoke(
            is_heap_t, Rng&& rng, Comp&& comp = Comp(), Proj&& proj = Proj())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng>::type;

            static_assert(
                (pika::traits::is_random_access_iterator<iterator_type>::value),
                "Requires a random access iterator.");

            return pika::parallel::detail::is_heap<iterator_type>().call(
                pika::execution::seq, pika::util::begin(rng),
                pika::util::end(rng), PIKA_FORWARD(Comp, comp),
                PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Iter, typename Sent,
            typename Comp = pika::parallel::detail::less,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_indirect_callable<
                    pika::execution::sequenced_policy, Comp,
                    pika::parallel::detail::projected<Proj, Iter>,
                    pika::parallel::detail::projected<Proj, Iter>
                >::value
            )>
        // clang-format on
        friend bool tag_fallback_invoke(is_heap_t, Iter first, Sent last,
            Comp&& comp = Comp(), Proj&& proj = Proj())
        {
            static_assert(
                (pika::traits::is_random_access_iterator<Iter>::value),
                "Requires a random access iterator.");

            return pika::parallel::detail::is_heap<Iter>().call(
                pika::execution::seq, first, last, PIKA_FORWARD(Comp, comp),
                PIKA_FORWARD(Proj, proj));
        }
    } is_heap{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::is_heap_until
    inline constexpr struct is_heap_until_t final
      : pika::detail::tag_parallel_algorithm<is_heap_until_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename Rng,
            typename Comp = pika::parallel::detail::less,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy, Comp,
                    pika::parallel::detail::projected_range<Proj, Rng>,
                    pika::parallel::detail::projected_range<Proj, Rng>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            typename pika::traits::range_iterator<Rng>::type>::type
        tag_fallback_invoke(is_heap_until_t, ExPolicy&& policy, Rng&& rng,
            Comp&& comp = Comp(), Proj&& proj = Proj())
        {
            using iterator_type =
                typename pika::traits::range_traits<Rng>::iterator_type;

            static_assert(
                (pika::traits::is_random_access_iterator<iterator_type>::value),
                "Requires a random access iterator.");

            return pika::parallel::detail::is_heap_until<iterator_type>().call(
                PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng),
                pika::util::end(rng), PIKA_FORWARD(Comp, comp),
                PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Iter, typename Sent,
            typename Comp = pika::parallel::detail::less,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy, Comp,
                    pika::parallel::detail::projected<Proj, Iter>,
                    pika::parallel::detail::projected<Proj, Iter>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            Iter>::type
        tag_fallback_invoke(is_heap_until_t, ExPolicy&& policy, Iter first,
            Sent last, Comp&& comp = Comp(), Proj&& proj = Proj())
        {
            static_assert(
                (pika::traits::is_random_access_iterator<Iter>::value),
                "Requires a random access iterator.");

            return pika::parallel::detail::is_heap_until<Iter>().call(
                PIKA_FORWARD(ExPolicy, policy), first, last,
                PIKA_FORWARD(Comp, comp), PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Rng,
            typename Comp = pika::parallel::detail::less,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::parallel::detail::is_indirect_callable<
                    pika::execution::sequenced_policy, Comp,
                    pika::parallel::detail::projected_range<Proj, Rng>,
                    pika::parallel::detail::projected_range<Proj, Rng>
                >::value
            )>
        // clang-format on
        friend typename pika::traits::range_iterator<Rng>::type
        tag_fallback_invoke(is_heap_until_t, Rng&& rng, Comp&& comp = Comp(),
            Proj&& proj = Proj())
        {
            using iterator_type =
                typename pika::traits::range_traits<Rng>::iterator_type;

            static_assert(
                (pika::traits::is_random_access_iterator<iterator_type>::value),
                "Requires a random access iterator.");

            return pika::parallel::detail::is_heap_until<iterator_type>().call(
                pika::execution::seq, pika::util::begin(rng),
                pika::util::end(rng), PIKA_FORWARD(Comp, comp),
                PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Iter, typename Sent,
            typename Comp = pika::parallel::detail::less,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_indirect_callable<
                    pika::execution::sequenced_policy, Comp,
                    pika::parallel::detail::projected<Proj, Iter>,
                    pika::parallel::detail::projected<Proj, Iter>
                >::value
            )>
        // clang-format on
        friend Iter tag_fallback_invoke(is_heap_until_t, Iter first, Sent last,
            Comp&& comp = Comp(), Proj&& proj = Proj())
        {
            static_assert(
                (pika::traits::is_random_access_iterator<Iter>::value),
                "Requires a random access iterator.");

            return pika::parallel::detail::is_heap_until<Iter>().call(
                pika::execution::seq, first, last, PIKA_FORWARD(Comp, comp),
                PIKA_FORWARD(Proj, proj));
        }
    } is_heap_until{};
}    // namespace pika::ranges

#endif    //DOXYGEN
