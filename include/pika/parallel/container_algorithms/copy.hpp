//  Copyright (c) 2015-2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/container_algorithms/copy.hpp

#pragma once

#if defined(DOXYGEN)

namespace pika { namespace ranges {
    // clang-format off

    /// Copies the elements in the range, defined by [first, last), to another
    /// range beginning at \a dest.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter1       The type of the begin source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Sent1       The type of the end source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     sentinel for Iter1.
    /// \tparam FwdIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param iter         Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param sent         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    ///
    /// The assignments in the parallel \a copy algorithm invoked with an
    /// execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a copy algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a copy algorithm returns a
    ///           \a pika::future<ranges::copy_result<FwdIter1, FwdIter> > if
    ///           the execution policy is of type
    ///           \a sequenced_task_policy or \a parallel_task_policy and
    ///           returns \a ranges::copy_result<FwdIter1, FwdIter> otherwise.
    ///           The \a copy algorithm returns the pair of the input iterator
    ///           \a last and the output iterator to the element in the
    ///           destination range, one past the last element copied.
    template <typename ExPolicy, typename Iter1, typename Sent1,
        typename FwdIter>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        pika::ranges::copy_result<Iter1, Iter>>::type
    copy(ExPolicy&& policy, Iter1 iter, Sent1 sent, FwdIter dest);

    /// Copies the elements in the range \a rng to another
    /// range beginning at \a dest.
    ///
    /// \note   Complexity: Performs exactly
    ///         std::distance(begin(rng), end(rng)) assignments.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an input iterator.
    /// \tparam FwdIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    ///
    /// The assignments in the parallel \a copy algorithm invoked with an
    /// execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a copy algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a copy algorithm returns a
    ///           \a pika::future<ranges::copy_result<iterator_t<Rng>, FwdIter2>>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or \a parallel_task_policy and
    ///           returns \a ranges::copy_result<iterator_t<Rng>, FwdIter2>
    ///           otherwise.
    ///           The \a copy algorithm returns the pair of the input iterator
    ///           \a last and the output iterator to the element in the
    ///           destination range, one past the last element copied.
    template <typename ExPolicy, typename Rng, typename FwdIter>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        pika::ranges::copy_result<
            typename pika::traits::range_traits<Rng>::iterator_type,
            FwdIter>
        >::type
    copy(ExPolicy&& policy, Rng&& rng, FwdIter dest);

    /// Copies the elements in the range [first, first + count), starting from
    /// first and proceeding to first + count - 1., to another range beginning
    /// at dest.
    ///
    /// \note   Complexity: Performs exactly \a count assignments, if
    ///         count > 0, no assignments otherwise.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter1    The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Size        The type of the argument specifying the number of
    ///                     elements to apply \a f to.
    /// \tparam FwdIter2    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param count        Refers to the number of elements starting at
    ///                     \a first the algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    ///
    /// The assignments in the parallel \a copy_n algorithm invoked with
    /// an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a copy_n algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a copy_n algorithm returns a
    ///           \a pika::future<ranges::copy_n_result<FwdIter1, FwdIter2> >
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a ranges::copy_n_result<FwdIter1, FwdIter2>
    ///           otherwise.
    ///           The \a copy algorithm returns the pair of the input iterator
    ///           forwarded to the first element after the last in the input
    ///           sequence and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename Size,
        typename FwdIter2>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        pika::ranges::copy_n_result<FwdIter1, FwdIter2>>::type
    copy_n(ExPolicy&& policy, FwdIter1 first, Size count, FwdIter2 dest);

    /// Copies the elements in the range, defined by [first, last) to another
    /// range beginning at \a dest. Copies only the elements for which the
    /// predicate \a f returns true. The order of the elements that are not
    /// removed is preserved.
    ///
    /// \note   Complexity: Performs not more than
    ///         std::distance(begin(rng), end(rng)) assignments,
    ///         exactly std::distance(begin(rng), end(rng)) applications
    ///         of the predicate \a f.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter1    The type of the begin source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Sent1       The type of the end source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     sentinel for FwdIter1.
    /// \tparam FwdIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    /// \tparam F           The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a copy_if requires \a F to meet the
    ///                     requirements of \a CopyConstructible.
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param iter         Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param sent         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
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
    ///                     type \a InIter can be dereferenced and then
    ///                     implicitly converted to Type.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a copy_if algorithm invoked with
    /// an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a copy_if algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a copy_if algorithm returns a
    ///           \a pika::future<ranges::copy_if_result<iterator_t<Rng>, FwdIter2>>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or \a parallel_task_policy and
    ///           returns \a ranges::copy_if_result<iterator_t<Rng>, FwdIter2>
    ///           otherwise.
    ///           The \a copy_if algorithm returns the pair of the input iterator
    ///           \a last and the output iterator to the element in the
    ///           destination range, one past the last element copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename Sent1,
        typename FwdIter, typename F,
        typename Proj = pika::parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        pika::ranges::copy_if_result<
            typename pika::traits::range_traits<Rng>::iterator_type,
            OutIter>>::type
    copy_if(ExPolicy&& policy, FwdIter1 iter, Sent1 sent, FwdIter dest, F&& f,
        Proj&& proj = Proj());

    /// Copies the elements in the range \a rng to another
    /// range beginning at \a dest. Copies only the elements for which the
    /// predicate \a f returns true. The order of the elements that are not
    /// removed is preserved.
    ///
    /// \note   Complexity: Performs not more than
    ///         std::distance(begin(rng), end(rng)) assignments,
    ///         exactly std::distance(begin(rng), end(rng)) applications
    ///         of the predicate \a f.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an input iterator.
    /// \tparam OutIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    /// \tparam F           The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a copy_if requires \a F to meet the
    ///                     requirements of \a CopyConstructible.
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
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
    ///                     type \a InIter can be dereferenced and then
    ///                     implicitly converted to Type.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a copy_if algorithm invoked with
    /// an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a copy_if algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a copy_if algorithm returns a
    ///           \a pika::future<ranges::copy_if_result<iterator_t<Rng>, FwdIter2>>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or \a parallel_task_policy and
    ///           returns \a ranges::copy_if_result<iterator_t<Rng>, FwdIter2>
    ///           otherwise.
    ///           The \a copy_if algorithm returns the pair of the input iterator
    ///           \a last and the output iterator to the element in the
    ///           destination range, one past the last element copied.
    ///
    template <typename ExPolicy, typename Rng, typename OutIter, typename F,
        typename Proj = pika::parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        pika::ranges::copy_if_result<
            typename pika::traits::range_traits<Rng>::iterator_type, OutIter>
    >::type
    copy_if(
        ExPolicy&& policy, Rng&& rng, OutIter dest, F&& f, Proj&& proj = Proj());

    // clang-format on
}}    // namespace pika::ranges

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/iterator_support/range.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/iterator_support/traits/is_range.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>

#include <pika/algorithms/traits/projected.hpp>
#include <pika/algorithms/traits/projected_range.hpp>
#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/algorithms/copy.hpp>
#include <pika/parallel/util/result_types.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace pika::ranges {
    template <typename I, typename O>
    using copy_result = pika::parallel::detail::in_out_result<I, O>;

    template <typename I, typename O>
    using copy_n_result = pika::parallel::detail::in_out_result<I, O>;

    template <typename I, typename O>
    using copy_if_result = pika::parallel::detail::in_out_result<I, O>;

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::copy
    inline constexpr struct copy_t final
      : pika::detail::tag_parallel_algorithm<copy_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename Sent1,
            typename FwdIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_sentinel_for<Sent1, FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            ranges::copy_result<FwdIter1, FwdIter>>::type
        tag_fallback_invoke(pika::ranges::copy_t, ExPolicy&& policy,
            FwdIter1 iter, Sent1 sent, FwdIter dest)
        {
            using copy_iter_t =
                pika::parallel::detail::copy_iter<FwdIter1, FwdIter>;

            return pika::parallel::detail::transfer<copy_iter_t>(
                PIKA_FORWARD(ExPolicy, policy), iter, sent, dest);
        }

        // clang-format off
        template <typename ExPolicy, typename Rng, typename FwdIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng>::value &&
                pika::traits::is_iterator<FwdIter>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            ranges::copy_result<
                typename pika::traits::range_traits<Rng>::iterator_type,
                FwdIter>>::type
        tag_fallback_invoke(
            pika::ranges::copy_t, ExPolicy&& policy, Rng&& rng, FwdIter dest)
        {
            using copy_iter_t = pika::parallel::detail::copy_iter<
                typename pika::traits::range_traits<Rng>::iterator_type,
                FwdIter>;

            return pika::parallel::detail::transfer<copy_iter_t>(
                PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng),
                pika::util::end(rng), dest);
        }

        // clang-format off
        template <typename FwdIter1, typename Sent1, typename FwdIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_sentinel_for<Sent1, FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter>::value
            )>
        // clang-format on
        friend ranges::copy_result<FwdIter1, FwdIter> tag_fallback_invoke(
            pika::ranges::copy_t, FwdIter1 iter, Sent1 sent, FwdIter dest)
        {
            using copy_iter_t =
                pika::parallel::detail::copy_iter<FwdIter1, FwdIter>;

            return pika::parallel::detail::transfer<copy_iter_t>(
                pika::execution::seq, iter, sent, dest);
        }

        // clang-format off
        template <typename Rng, typename FwdIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_range<Rng>::value &&
                pika::traits::is_iterator<FwdIter>::value
            )>
        // clang-format on
        friend ranges::copy_result<
            typename pika::traits::range_traits<Rng>::iterator_type, FwdIter>
        tag_fallback_invoke(pika::ranges::copy_t, Rng&& rng, FwdIter dest)
        {
            using copy_iter_t = pika::parallel::detail::copy_iter<
                typename pika::traits::range_traits<Rng>::iterator_type,
                FwdIter>;

            return pika::parallel::detail::transfer<copy_iter_t>(
                pika::execution::seq, pika::util::begin(rng),
                pika::util::end(rng), dest);
        }
    } copy{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::copy_n
    inline constexpr struct copy_n_t final
      : pika::detail::tag_parallel_algorithm<copy_n_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename Size,
            typename FwdIter2,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter2>::value)>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            ranges::copy_n_result<FwdIter1, FwdIter2>>::type
        tag_fallback_invoke(pika::ranges::copy_n_t, ExPolicy&& policy,
            FwdIter1 first, Size count, FwdIter2 dest)
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter1>::value),
                "Required at least forward iterator.");
            static_assert(pika::traits::is_forward_iterator<FwdIter2>::value ||
                    pika::is_sequenced_execution_policy<ExPolicy>::value,
                "Requires at least forward iterator or sequential execution.");

            // if count is representing a negative value, we do nothing
            if (pika::parallel::detail::is_negative(count))
            {
                return pika::parallel::detail::algorithm_result<ExPolicy,
                    ranges::copy_n_result<FwdIter1, FwdIter2>>::
                    get(ranges::copy_n_result<FwdIter1, FwdIter2>{
                        PIKA_MOVE(first), PIKA_MOVE(dest)});
            }

            return pika::parallel::detail::copy_n_algo<
                ranges::copy_n_result<FwdIter1, FwdIter2>>()
                .call(PIKA_FORWARD(ExPolicy, policy), first, std::size_t(count),
                    dest);
        }

        // clang-format off
        template <typename FwdIter1, typename Size, typename FwdIter2,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter2>::value)>
        // clang-format on
        friend ranges::copy_n_result<FwdIter1, FwdIter2> tag_fallback_invoke(
            pika::ranges::copy_n_t, FwdIter1 first, Size count, FwdIter2 dest)
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter1>::value),
                "Required at least forward iterator.");
            static_assert((pika::traits::is_output_iterator<FwdIter2>::value),
                "Requires at least output iterator.");

            // if count is representing a negative value, we do nothing
            if (pika::parallel::detail::is_negative(count))
            {
                return ranges::copy_n_result<FwdIter1, FwdIter2>{
                    PIKA_MOVE(first), PIKA_MOVE(dest)};
            }

            return pika::parallel::detail::copy_n_algo<
                ranges::copy_n_result<FwdIter1, FwdIter2>>()
                .call(pika::execution::seq, first, std::size_t(count), dest);
        }
    } copy_n{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::copy_if
    inline constexpr struct copy_if_t final
      : pika::detail::tag_parallel_algorithm<copy_if_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename Sent1,
            typename FwdIter, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_sentinel_for<Sent1, FwdIter1>::value &&
                pika::parallel::detail::is_projected<Proj, FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy, Pred,
                    pika::parallel::detail::projected<Proj, FwdIter1>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            ranges::copy_if_result<FwdIter1, FwdIter>>::type
        tag_fallback_invoke(pika::ranges::copy_if_t, ExPolicy&& policy,
            FwdIter1 iter, Sent1 sent, FwdIter dest, Pred&& pred,
            Proj&& proj = Proj())
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter1>::value),
                "Required at least forward iterator.");

            static_assert(pika::traits::is_forward_iterator<FwdIter>::value ||
                    (pika::is_sequenced_execution_policy<ExPolicy>::value &&
                        pika::traits::is_output_iterator<FwdIter>::value),
                "Requires at least forward iterator or sequential execution.");

            return pika::parallel::detail::copy_if_algo<
                pika::parallel::detail::in_out_result<FwdIter1, FwdIter>>()
                .call(PIKA_FORWARD(ExPolicy, policy), iter, sent, dest,
                    PIKA_FORWARD(Pred, pred), PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Rng, typename FwdIter,
            typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::traits::is_iterator<FwdIter>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy, Pred,
                    pika::parallel::detail::projected_range<Proj, Rng>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            ranges::copy_if_result<
                typename pika::traits::range_traits<Rng>::iterator_type,
                FwdIter>>::type
        tag_fallback_invoke(pika::ranges::copy_if_t, ExPolicy&& policy,
            Rng&& rng, FwdIter dest, Pred&& pred, Proj&& proj = Proj())
        {
            static_assert(pika::traits::is_forward_iterator<FwdIter>::value ||
                    (pika::is_sequenced_execution_policy<ExPolicy>::value &&
                        pika::traits::is_output_iterator<FwdIter>::value),
                "Requires at least forward iterator or sequential execution.");

            return pika::parallel::detail::copy_if_algo<
                pika::parallel::detail::in_out_result<
                    typename pika::traits::range_traits<Rng>::iterator_type,
                    FwdIter>>()
                .call(PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng),
                    pika::util::end(rng), dest, PIKA_FORWARD(Pred, pred),
                    PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename FwdIter1, typename Sent1, typename FwdIter,
            typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_sentinel_for<Sent1, FwdIter1>::value &&
                pika::parallel::detail::is_projected<Proj, FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter>::value &&
                pika::parallel::detail::is_indirect_callable<
                    pika::execution::sequenced_policy, Pred,
                    pika::parallel::detail::projected<Proj, FwdIter1>
                >::value
            )>
        // clang-format on
        friend ranges::copy_if_result<FwdIter1, FwdIter>
        tag_fallback_invoke(pika::ranges::copy_if_t, FwdIter1 iter, Sent1 sent,
            FwdIter dest, Pred&& pred, Proj&& proj = Proj())
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter1>::value),
                "Required at least forward iterator.");

            static_assert((pika::traits::is_output_iterator<FwdIter>::value),
                "Required at least output iterator.");

            return pika::parallel::detail::copy_if_algo<
                pika::parallel::detail::in_out_result<FwdIter1, FwdIter>>()
                .call(pika::execution::seq, iter, sent, dest,
                    PIKA_FORWARD(Pred, pred), PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Rng, typename FwdIter, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::traits::is_iterator<FwdIter>::value &&
                pika::parallel::detail::is_indirect_callable<
                    pika::execution::sequenced_policy, Pred,
                    pika::parallel::detail::projected_range<Proj, Rng>
                >::value
            )>
        // clang-format on
        friend ranges::copy_if_result<
            typename pika::traits::range_traits<Rng>::iterator_type, FwdIter>
        tag_fallback_invoke(pika::ranges::copy_if_t, Rng&& rng, FwdIter dest,
            Pred&& pred, Proj&& proj = Proj())
        {
            static_assert((pika::traits::is_output_iterator<FwdIter>::value),
                "Required at least output iterator.");

            return pika::parallel::detail::copy_if_algo<
                pika::parallel::detail::in_out_result<
                    typename pika::traits::range_traits<Rng>::iterator_type,
                    FwdIter>>()
                .call(pika::execution::seq, pika::util::begin(rng),
                    pika::util::end(rng), dest, PIKA_FORWARD(Pred, pred),
                    PIKA_FORWARD(Proj, proj));
        }

    } copy_if{};
}    // namespace pika::ranges
#endif    // DOXYGEN
