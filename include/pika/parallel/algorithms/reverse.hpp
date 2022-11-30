//  Copyright (c) 2007-2017 Hartmut Kaiser
//  Copyright (c)      2021 Giannis Gonidelis
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/reverse.hpp

#pragma once

#if defined(DOXYGEN)

namespace pika {

    /// Reverses the order of the elements in the range [first, last).
    /// Behaves as if applying std::iter_swap to every pair of iterators
    /// first+i, (last-i) - 1 for each non-negative i < (last-first)/2.
    ///
    /// \note   Complexity: Linear in the distance between \a first and \a last.
    ///
    /// \tparam BidirIter  The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     bidirectional iterator.
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    ///
    /// The assignments in the parallel \a reverse algorithm
    /// execute in sequential order in the calling thread.
    ///
    /// \returns  The \a reverse algorithm returns a \a void.
    ///
    template <typename BidirIter>
    void reverse(BidirIter first, BidirIter last);

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
    /// \tparam BidirIter  The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     bidirectional iterator.
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
    /// \returns  The \a reverse algorithm returns a \a pika::future<void>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a void otherwise.
    ///
    template <typename ExPolicy, typename BidirIter>
    typename parallel::detail::algorithm_result<ExPolicy, void>::type
    reverse(ExPolicy&& policy, BidirIter first, BidirIter last);

    ///////////////////////////////////////////////////////////////////////////
    /// Copies the elements from the range [first, last) to another range
    /// beginning at dest_first in such a way that the elements in the new
    /// range are in reverse order.
    /// Behaves as if by executing the assignment
    /// *(dest_first + (last - first) - 1 - i) = *(first + i) once for each
    /// non-negative i < (last - first)
    /// If the source and destination ranges (that is, [first, last) and
    /// [dest_first, dest_first+(last-first)) respectively) overlap, the
    /// behavior is undefined.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam BidirIter   The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     bidirectional iterator.
    /// \tparam OutIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the begin of the destination range.
    ///
    /// The assignments in the parallel \a reverse_copy algorithm
    /// execute in sequential order in the calling thread.
    ///
    /// \returns  The \a reverse_copy algorithm returns an
    ///           \a OutIter.
    ///           The \a reverse_copy algorithm returns the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename BidirIter, typename OutIter>
    OutIter reverse_copy(BidirIter first, BidirIter last, Outiter dest);

    /// Copies the elements from the range [first, last) to another range
    /// beginning at dest_first in such a way that the elements in the new
    /// range are in reverse order.
    /// Behaves as if by executing the assignment
    /// *(dest_first + (last - first) - 1 - i) = *(first + i) once for each
    /// non-negative i < (last - first)
    /// If the source and destination ranges (that is, [first, last) and
    /// [dest_first, dest_first+(last-first)) respectively) overlap, the
    /// behavior is undefined.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam BidirIter   The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     bidirectional iterator.
    /// \tparam FwdIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the begin of the destination range.
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
    ///           \a pika::future<FwdIter>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a FwdIter
    ///           otherwise.
    ///           The \a reverse_copy algorithm returns the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename BidirIter, typename FwdIter>
    typename pika::parallel::detail::algorithm_result<ExPolicy, FwdIter>::type
    reverse_copy(
        ExPolicy&& policy, BidirIter first, BidirIter last, FwdIter dest_first);

}    // namespace pika

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>

#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/algorithms/copy.hpp>
#include <pika/parallel/algorithms/detail/advance_to_sentinel.hpp>
#include <pika/parallel/algorithms/detail/dispatch.hpp>
#include <pika/parallel/algorithms/for_each.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/projection_identity.hpp>
#include <pika/parallel/util/ranges_facilities.hpp>
#include <pika/parallel/util/result_types.hpp>
#include <pika/parallel/util/zip_iterator.hpp>

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>

namespace pika::parallel::detail {
    ///////////////////////////////////////////////////////////////////////////
    // reverse
    /// \cond NOINTERNAL
    template <typename Iter>
    struct reverse : public algorithm<reverse<Iter>, Iter>
    {
        reverse()
          : reverse::algorithm("reverse")
        {
        }

        template <typename ExPolicy, typename BidirIter, typename Sent>
        constexpr static BidirIter
        sequential(ExPolicy, BidirIter first, Sent last)
        {
            auto last2{pika::ranges::next(first, last)};
            for (auto tail{last2}; !(first == tail || first == --tail); ++first)
            {
#if defined(PIKA_ALGORITHMS_HAVE_CXX20_STD_RANGES_ITER_SWAP)
                std::ranges::iter_swap(first, tail);
#else
                std::iter_swap(first, tail);
#endif
            }
            return last2;
        }

        template <typename ExPolicy, typename BidirIter, typename Sent>
        static typename algorithm_result<ExPolicy, BidirIter>::type
        parallel(ExPolicy&& policy, BidirIter first, Sent last)
        {
            auto last2{pika::ranges::next(first, last)};
            using destination_iterator = std::reverse_iterator<BidirIter>;
            using zip_iterator =
                pika::util::zip_iterator<BidirIter, destination_iterator>;
            using reference = typename zip_iterator::reference;

            return convert_to_result(for_each_n<zip_iterator>().call(
                                         PIKA_FORWARD(ExPolicy, policy),
                                         pika::util::make_zip_iterator(first,
                                             destination_iterator(last2)),
                                         (distance) (first, last2) / 2,
                                         [](reference t) -> void {
                                             using std::get;
                                             std::swap(get<0>(t), get<1>(t));
                                         },
                                         projection_identity()),
                [last2](zip_iterator const&) -> BidirIter { return last2; });
        }
    };
    /// \endcond

    ///////////////////////////////////////////////////////////////////////////
    // reverse_copy
    /// \cond NOINTERNAL

    // sequential reverse_copy
    template <typename BidirIt, typename Sent, typename OutIter>
    constexpr inline in_out_result<BidirIt, OutIter>
    sequential_reverse_copy(BidirIt first, Sent last, OutIter dest)
    {
        auto iter{pika::ranges::next(first, last)};
        while (first != iter)
        {
            *dest++ = *--iter;
        }
        return in_out_result<BidirIt, OutIter>{iter, dest};
    }

    template <typename IterPair>
    struct reverse_copy : public algorithm<reverse_copy<IterPair>, IterPair>
    {
        reverse_copy()
          : reverse_copy::algorithm("reverse_copy")
        {
        }

        template <typename ExPolicy, typename BidirIter, typename Sent,
            typename OutIter>
        constexpr static in_out_result<BidirIter, OutIter>
        sequential(ExPolicy, BidirIter first, Sent last, OutIter dest_first)
        {
            return sequential_reverse_copy(first, last, dest_first);
        }

        template <typename ExPolicy, typename BidirIter, typename Sent,
            typename FwdIter>
        static typename algorithm_result<ExPolicy,
            in_out_result<BidirIter, FwdIter>>::type
        parallel(
            ExPolicy&& policy, BidirIter first, Sent last, FwdIter dest_first)
        {
            auto last2{pika::ranges::next(first, last)};
            using iterator = std::reverse_iterator<BidirIter>;

            return convert_to_result(
                copy_algo<in_out_result<iterator, FwdIter>>().call(
                    PIKA_FORWARD(ExPolicy, policy), iterator(last2),
                    iterator(first), dest_first),
                [](in_out_result<iterator, FwdIter> const& p)
                    -> in_out_result<BidirIter, FwdIter> {
                    return in_out_result<BidirIter, FwdIter>{
                        p.in.base(), p.out};
                });
        }
    };
    /// \endcond
}    // namespace pika::parallel::detail

namespace pika {
    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::reverse
    inline constexpr struct reverse_t final
      : pika::detail::tag_parallel_algorithm<reverse_t>
    {
    private:
        // clang-format off
        template <typename BidirIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<BidirIter>::value
            )>
        // clang-format on
        friend void
        tag_fallback_invoke(pika::reverse_t, BidirIter first, BidirIter last)
        {
            static_assert(
                (pika::traits::is_bidirectional_iterator<BidirIter>::value),
                "Requires at least bidirectional iterator.");

            pika::parallel::detail::reverse<BidirIter>().call(
                pika::execution::sequenced_policy{}, first, last);
        }

        // clang-format off
        template <typename ExPolicy, typename BidirIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<BidirIter>::value &&
                pika::is_execution_policy<ExPolicy>::value
            )>
        // clang-format on
        friend typename parallel::detail::algorithm_result<ExPolicy, void>::type
        tag_fallback_invoke(
            pika::reverse_t, ExPolicy&& policy, BidirIter first, BidirIter last)
        {
            static_assert(
                (pika::traits::is_bidirectional_iterator<BidirIter>::value),
                "Requires at least bidirectional iterator.");

            return parallel::detail::algorithm_result<ExPolicy>::get(
                pika::parallel::detail::reverse<BidirIter>().call(
                    PIKA_FORWARD(ExPolicy, policy), first, last));
        }
    } reverse{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::reverse_copy
    inline constexpr struct reverse_copy_t final
      : pika::detail::tag_parallel_algorithm<reverse_copy_t>
    {
    private:
        // clang-format off
        template <typename BidirIter, typename OutIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<BidirIter>::value &&
                pika::traits::is_iterator<OutIter>::value
            )>
        // clang-format on
        friend OutIter tag_fallback_invoke(
            pika::reverse_copy_t, BidirIter first, BidirIter last, OutIter dest)
        {
            static_assert(
                (pika::traits::is_bidirectional_iterator<BidirIter>::value),
                "Requires at least bidirectional iterator.");

            static_assert((pika::traits::is_output_iterator<OutIter>::value),
                "Requires at least output iterator.");

            return parallel::detail::get_second_element(
                parallel::detail::reverse_copy<
                    pika::parallel::detail::in_out_result<BidirIter, OutIter>>()
                    .call(pika::execution::sequenced_policy{}, first, last,
                        dest));
        }

        // clang-format off
        template <typename ExPolicy, typename BidirIter, typename FwdIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<BidirIter>::value &&
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter>::value
            )>
        // clang-format on
        friend
            typename parallel::detail::algorithm_result<ExPolicy, FwdIter>::type
            tag_fallback_invoke(pika::reverse_copy_t, ExPolicy&& policy,
                BidirIter first, BidirIter last, FwdIter dest)
        {
            static_assert(
                (pika::traits::is_bidirectional_iterator<BidirIter>::value),
                "Requires at least bidirectional iterator.");

            static_assert((pika::traits::is_forward_iterator<FwdIter>::value),
                "Requires at least forward iterator.");

            return parallel::detail::get_second_element(
                parallel::detail::reverse_copy<
                    pika::parallel::detail::in_out_result<BidirIter, FwdIter>>()
                    .call(PIKA_FORWARD(ExPolicy, policy), first, last, dest));
        }
    } reverse_copy{};
}    // namespace pika

#endif    // DOXYGEN
