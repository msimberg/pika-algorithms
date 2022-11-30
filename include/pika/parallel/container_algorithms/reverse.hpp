//  Copyright (c) 2007-2015 Hartmut Kaiser
//  Copyright (c)      2021 Giannis Gonidelis
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/container_algorithms/reverse.hpp

#pragma once

#if defined(DOXYGEN)

namespace pika { namespace ranges {

    ///////////////////////////////////////////////////////////////////////////
    /// Reverses the order of the elements in the range [first, last).
    /// Behaves as if applying std::iter_swap to every pair of iterators
    /// first+i, (last-i) - 1 for each non-negative i < (last-first)/2.
    ///
    /// \note   Complexity: Linear in the distance between \a first and \a last.
    ///
    /// \tparam Iter        The type of the source iterator used (deduced).
    ///                     The iterator type must
    ///                     meet the requirements of an input iterator.
    /// \tparam Sent        The type of the end iterators used (deduced). This
    ///                     sentinel type must be a sentinel for Iter.
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    ///
    /// The assignments in the parallel \a reverse algorithm
    /// execute in sequential order in the calling thread.
    ///
    ///
    /// \returns  The \a reverse algorithm returns a \a Iter.
    ///           It returns \a last.
    ///
    template <typename Iter, typename Sent>
    Iter reverse(Iter first, Sent last);

    /// Uses \a rng as the source range, as if using \a util::begin(rng) as
    /// \a first and \a ranges::end(rng) as \a last.
    /// Reverses the order of the elements in the range [first, last).
    /// Behaves as if applying std::iter_swap to every pair of iterators
    /// first+i, (last-i) - 1 for each non-negative i < (last-first)/2.
    ///
    /// \note   Complexity: Linear in the distance between \a first and \a last.
    ///
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of a bidirectional iterator.
    ///
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    ///
    /// The assignments in the parallel \a reverse algorithm
    /// execute in sequential order in the calling thread.
    ///
    ///
    /// \returns  The \a reverse algorithm returns a
    ///           \a pika::traits::range_iterator<Rng>::type.
    ///           It returns \a last.
    ///
    template <typename Rng>
    typename pika::traits::range_iterator<Rng>::type reverse(Rng&& rng);

    /// Reverses the order of the elements in the range [first, last).
    /// Behaves as if applying std::iter_swap to every pair of iterators
    /// first+i, (last-i) - 1 for each non-negative i < (last-first)/2.
    ///
    /// \note   Complexity: Linear in the distance between \a first and \a last.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter        The type of the source iterator used (deduced).
    ///                     The iterator type must
    ///                     meet the requirements of an input iterator.
    /// \tparam Sent        The type of the end iterators used (deduced). This
    ///                     sentinel type must be a sentinel for Iter.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    ///
    /// The assignments in the parallel \a reverse algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a reverse algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a reverse algorithm returns a \a pika::future<Iter>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a Iter otherwise.
    ///           It returns \a last.
    ///
    template <typename ExPolicy, typename Iter, typename Sent>
    typename pika::parallel::detail::algorithm_result<ExPolicy, Iter>::type
    reverse(ExPolicy&& policy, Iter first, Sent last);

    /// Uses \a rng as the source range, as if using \a util::begin(rng) as
    /// \a first and \a ranges::end(rng) as \a last.
    /// Reverses the order of the elements in the range [first, last).
    /// Behaves as if applying std::iter_swap to every pair of iterators
    /// first+i, (last-i) - 1 for each non-negative i < (last-first)/2.
    ///
    /// \note   Complexity: Linear in the distance between \a first and \a last.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of a bidirectional iterator.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    ///
    /// The assignments in the parallel \a reverse algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a reverse algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a reverse algorithm returns a
    ///           \a pika::future<typename pika::traits::range_iterator<Rng>::type>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a pika::future<
    ///             typename pika::traits::range_iterator<Rng>::type> otherwise.
    ///           It returns \a last.
    ///
    template <typename ExPolicy, typename Rng>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        typename pika::traits::range_iterator<Rng>::type>::type
    reverse(ExPolicy&& policy, Rng&& rng);

    ///////////////////////////////////////////////////////////////////////////
    /// Copies the elements from the range [first, last) to another range
    /// beginning at result in such a way that the elements in the new
    /// range are in reverse order.
    /// Behaves as if by executing the assignment
    /// *(result + (last - first) - 1 - i) = *(first + i) once for each
    /// non-negative i < (last - first)
    /// If the source and destination ranges (that is, [first, last) and
    /// [result, result+(last-first)) respectively) overlap, the
    /// behavior is undefined.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam Iter        The type of the source iterator used (deduced).
    ///                     The iterator type must
    ///                     meet the requirements of an input iterator.
    /// \tparam Sent        The type of the end iterators used (deduced). This
    ///                     sentinel type must be a sentinel for Iter.
    /// \tparam OutIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param result   Refers to the begin of the destination range.
    ///
    /// The assignments in the parallel \a reverse_copy algorithm
    /// execute in sequential order in the calling thread.
    ///
    ///
    /// \returns  The \a reverse_copy algorithm returns a
    ///           \a reverse_copy_result<Iter, OutIter>.
    ///           The \a reverse_copy algorithm returns the pair of the input iterator
    ///           forwarded to the first element after the last in the input
    ///           sequence and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename Iter, typename Sent, typename OutIter>
    reverse_copy_result<Iter, OutIter>
    reverse_copy(Iter first, Sent last, OutIter result);

    /// Uses \a rng as the source range, as if using \a util::begin(rng) as
    /// \a first and \a ranges::end(rng) as \a last.
    /// Copies the elements from the range [first, last) to another range
    /// beginning at result in such a way that the elements in the new
    /// range are in reverse order.
    /// Behaves as if by executing the assignment
    /// *(result + (last - first) - 1 - i) = *(first + i) once for each
    /// non-negative i < (last - first)
    /// If the source and destination ranges (that is, [first, last) and
    /// [result, result+(last-first)) respectively) overlap, the
    /// behavior is undefined.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of a bidirectional iterator.
    /// \tparam OutputIter  The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    ///
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param result       Refers to the begin of the destination range.
    ///
    /// The assignments in the parallel \a reverse_copy algorithm
    /// execute in sequential order in the calling thread.
    ///
    /// \returns  The \a reverse_copy algorithm returns a
    ///           \a ranges::reverse_copy_result<
    ///           typename pika::traits::range_iterator<Rng>::type, OutIter>>::type.
    ///           The \a reverse_copy algorithm returns
    ///           an object equal to {last, result + N} where N = last - first
    ///
    template <typename Rng, typename OutIter>
    typename ranges::reverse_copy_result<
        typename pika::traits::range_iterator<Rng>::type, OutIter>
    reverse_copy(Rng&& rng, OutIter result);

    /// Copies the elements from the range [first, last) to another range
    /// beginning at result in such a way that the elements in the new
    /// range are in reverse order.
    /// Behaves as if by executing the assignment
    /// *(result + (last - first) - 1 - i) = *(first + i) once for each
    /// non-negative i < (last - first)
    /// If the source and destination ranges (that is, [first, last) and
    /// [result, result+(last-first)) respectively) overlap, the
    /// behavior is undefined.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter        The type of the source iterator used (deduced).
    ///                     The iterator type must
    ///                     meet the requirements of an input iterator.
    /// \tparam Sent        The type of the end iterators used (deduced). This
    ///                     sentinel type must be a sentinel for Iter.
    /// \tparam OutIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param result   Refers to the begin of the destination range.
    ///
    /// The assignments in the parallel \a reverse_copy algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a reverse_copy algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a reverse_copy algorithm returns a
    ///           \a pika::future<reverse_copy_result<Iter, OutIter> >
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a reverse_copy_result<Iter, OutIter>
    ///           otherwise.
    ///           The \a reverse_copy algorithm returns the pair of the input iterator
    ///           forwarded to the first element after the last in the input
    ///           sequence and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename Iter, typename Sent, typename OutIter>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        reverse_copy_result<Iter, OutIter>>::type
    reverse_copy(ExPolicy&& policy, Iter first, Sent last, OutIter result);

    /// Uses \a rng as the source range, as if using \a util::begin(rng) as
    /// \a first and \a ranges::end(rng) as \a last.
    /// Copies the elements from the range [first, last) to another range
    /// beginning at result in such a way that the elements in the new
    /// range are in reverse order.
    /// Behaves as if by executing the assignment
    /// *(result + (last - first) - 1 - i) = *(first + i) once for each
    /// non-negative i < (last - first)
    /// If the source and destination ranges (that is, [first, last) and
    /// [result, result+(last-first)) respectively) overlap, the
    /// behavior is undefined.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of a bidirectional iterator.
    /// \tparam OutputIter  The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param result   Refers to the begin of the destination range.
    ///
    /// The assignments in the parallel \a reverse_copy algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a reverse_copy algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a reverse_copy algorithm returns a
    ///           \a pika::future<ranges::reverse_copy_result<
    ///            typename pika::traits::range_iterator<Rng>::type, OutIter>>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a ranges::reverse_copy_result<
    ///            typename pika::traits::range_iterator<Rng>::type, OutIter>
    ///           otherwise.
    ///           The \a reverse_copy algorithm returns
    ///           an object equal to {last, result + N} where N = last - first
    ///
    template <typename ExPolicy, typename Rng, typename OutIter>
    typename algorithm_result<ExPolicy,
        ranges::reverse_copy_result<
            typename pika::traits::range_iterator<Rng>::type, OutIter>>::type
    reverse_copy(ExPolicy&& policy, Rng&& rng, OutIter result);
}}    // namespace pika::ranges

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/iterator_support/range.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/iterator_support/traits/is_range.hpp>
#include <pika/parallel/util/result_types.hpp>

#include <pika/algorithms/traits/projected_range.hpp>
#include <pika/parallel/algorithms/reverse.hpp>
#include <pika/parallel/util/projection_identity.hpp>

#include <type_traits>
#include <utility>

namespace pika::ranges {
    /// `reverse_copy_result` is equivalent to
    /// `pika::parallel::detail::in_out_result`
    template <typename I, typename O>
    using reverse_copy_result = pika::parallel::detail::in_out_result<I, O>;

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::reverse
    inline constexpr struct reverse_t final
      : pika::detail::tag_parallel_algorithm<reverse_t>
    {
    private:
        // clang-format off
        template <typename Iter, typename Sent,
        PIKA_CONCEPT_REQUIRES_(
            pika::traits::is_iterator<Iter>::value &&
            pika::traits::is_sentinel_for<Sent, Iter>::value
        )>
        // clang-format on
        friend Iter
        tag_fallback_invoke(pika::ranges::reverse_t, Iter first, Sent sent)
        {
            static_assert(
                (pika::traits::is_bidirectional_iterator<Iter>::value),
                "Required at least biderectional iterator.");

            return parallel::detail::reverse<Iter>().call(
                pika::execution::sequenced_policy{}, first, sent);
        }

        // clang-format off
        template <typename Rng,
        PIKA_CONCEPT_REQUIRES_(
            pika::traits::is_range<Rng>::value
        )>
        // clang-format on
        friend typename pika::traits::range_iterator<Rng>::type
        tag_fallback_invoke(pika::ranges::reverse_t, Rng&& rng)
        {
            static_assert(
                (pika::traits::is_bidirectional_iterator<
                    typename pika::traits::range_iterator<Rng>::type>::value),
                "Required at least biderectional iterator.");

            return parallel::detail::reverse<
                typename pika::traits::range_iterator<Rng>::type>()
                .call(pika::execution::sequenced_policy{},
                    pika::util::begin(rng), pika::util::end(rng));
        }

        // clang-format off
        template <typename ExPolicy, typename Iter, typename Sent,
        PIKA_CONCEPT_REQUIRES_(
            pika::is_execution_policy<ExPolicy>::value &&
            pika::traits::is_iterator<Iter>::value &&
            pika::traits::is_sentinel_for<Sent, Iter>::value
        )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            Iter>::type
        tag_fallback_invoke(
            pika::ranges::reverse_t, ExPolicy&& policy, Iter first, Sent sent)
        {
            static_assert(
                (pika::traits::is_bidirectional_iterator<Iter>::value),
                "Required at least biderectional iterator.");

            return parallel::detail::reverse<Iter>().call(
                PIKA_FORWARD(ExPolicy, policy), first, sent);
        }

        // clang-format off
        template <typename ExPolicy, typename Rng,
        PIKA_CONCEPT_REQUIRES_(
            pika::is_execution_policy<ExPolicy>::value &&
            pika::traits::is_range<Rng>::value
        )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            typename pika::traits::range_iterator<Rng>::type>::type
        tag_fallback_invoke(
            pika::ranges::reverse_t, ExPolicy&& policy, Rng&& rng)
        {
            static_assert(
                (pika::traits::is_bidirectional_iterator<
                    typename pika::traits::range_iterator<Rng>::type>::value),
                "Required at least biderectional iterator.");

            return parallel::detail::reverse<
                typename pika::traits::range_iterator<Rng>::type>()
                .call(PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng),
                    pika::util::end(rng));
        }
    } reverse{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::reverse_copy
    inline constexpr struct reverse_copy_t final
      : pika::detail::tag_parallel_algorithm<reverse_copy_t>
    {
    private:
        // clang-format off
        template <typename Iter, typename Sent, typename OutIter,
        PIKA_CONCEPT_REQUIRES_(
            pika::traits::is_iterator<Iter>::value &&
            pika::traits::is_sentinel_for<Sent, Iter>::value &&
            pika::traits::is_iterator<OutIter>::value
        )>
        // clang-format on
        friend reverse_copy_result<Iter, OutIter> tag_fallback_invoke(
            pika::ranges::reverse_copy_t, Iter first, Sent last, OutIter result)
        {
            static_assert(
                (pika::traits::is_bidirectional_iterator<Iter>::value),
                "Required at least biderectional iterator.");

            static_assert((pika::traits::is_output_iterator<OutIter>::value),
                "Required at least output iterator.");

            return parallel::detail::reverse_copy<
                pika::parallel::detail::in_out_result<Iter, OutIter>>()
                .call(pika::execution::sequenced_policy{}, first, last, result);
        }

        // clang-format off
        template <typename Rng, typename OutIter,
        PIKA_CONCEPT_REQUIRES_(
            pika::traits::is_range<Rng>::value &&
            pika::traits::is_iterator<OutIter>::value
        )>
        // clang-format on
        friend reverse_copy_result<
            typename pika::traits::range_iterator<Rng>::type, OutIter>
        tag_fallback_invoke(
            pika::ranges::reverse_copy_t, Rng&& rng, OutIter result)
        {
            static_assert(
                (pika::traits::is_bidirectional_iterator<
                    typename pika::traits::range_iterator<Rng>::type>::value),
                "Required at least biderectional iterator.");

            static_assert((pika::traits::is_output_iterator<OutIter>::value),
                "Required at least output iterator.");

            return parallel::detail::reverse_copy<
                pika::parallel::detail::in_out_result<
                    typename pika::traits::range_iterator<Rng>::type,
                    OutIter>>()
                .call(pika::execution::sequenced_policy{},
                    pika::util::begin(rng), pika::util::end(rng), result);
        }

        // clang-format off
        template <typename ExPolicy, typename Iter, typename Sent, typename FwdIter,
        PIKA_CONCEPT_REQUIRES_(
            pika::is_execution_policy<ExPolicy>::value &&
            pika::traits::is_iterator<Iter>::value &&
            pika::traits::is_sentinel_for<Sent, Iter>::value &&
            pika::traits::is_iterator<FwdIter>::value
        )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            reverse_copy_result<Iter, FwdIter>>::type
        tag_fallback_invoke(pika::ranges::reverse_copy_t, ExPolicy&& policy,
            Iter first, Sent last, FwdIter result)
        {
            static_assert(
                (pika::traits::is_bidirectional_iterator<Iter>::value),
                "Required at least biderectional iterator.");

            static_assert((pika::traits::is_forward_iterator<FwdIter>::value),
                "Required at least forward iterator.");

            return parallel::detail::reverse_copy<
                pika::parallel::detail::in_out_result<Iter, FwdIter>>()
                .call(PIKA_FORWARD(ExPolicy, policy), first, last, result);
        }

        // clang-format off
        template <typename ExPolicy, typename Rng, typename OutIter,
        PIKA_CONCEPT_REQUIRES_(
            pika::is_execution_policy<ExPolicy>::value &&
            pika::traits::is_range<Rng>::value &&
            pika::traits::is_iterator<OutIter>::value
        )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            reverse_copy_result<
                typename pika::traits::range_iterator<Rng>::type,
                OutIter>>::type
        tag_fallback_invoke(pika::ranges::reverse_copy_t, ExPolicy&& policy,
            Rng&& rng, OutIter result)
        {
            static_assert(
                (pika::traits::is_bidirectional_iterator<
                    typename pika::traits::range_iterator<Rng>::type>::value),
                "Required at least biderectional iterator.");

            static_assert((pika::traits::is_output_iterator<OutIter>::value),
                "Required at least output iterator.");

            return parallel::detail::reverse_copy<
                pika::parallel::detail::in_out_result<
                    typename pika::traits::range_iterator<Rng>::type,
                    OutIter>>()
                .call(PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng),
                    pika::util::end(rng), result);
        }
    } reverse_copy{};
}    // namespace pika::ranges

#endif    // DOXYGEN
