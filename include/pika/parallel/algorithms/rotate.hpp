//  Copyright (c) 2007-2017 Hartmut Kaiser
//  Copyright (c)      2021 Chuanqiu He
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
/// \file parallel/algorithms/rotate.hpp

#pragma once

#if defined(DOXYGEN)
namespace pika {
    /// Performs a left rotation on a range of elements. Specifically,
    /// \a rotate swaps the elements in the range [first, last) in such a way
    /// that the element new_first becomes the first element of the new range
    /// and new_first - 1 becomes the last element.
    ///
    /// \note   Complexity: Linear in the distance between \a first and \a last.
    ///
    /// \tparam FwdIter     The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param new_first    Refers to the element that should appear at the
    ///                     beginning of the rotated range.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// The assignments in the parallel \a rotate algorithm
    /// execute in sequential order in the calling thread.
    ///
    /// \note The type of dereferenced \a FwdIter must meet the requirements
    ///       of \a MoveAssignable and \a MoveConstructible.
    ///
    /// \returns  The \a rotate algorithm returns a FwdIter.
    ///           The \a rotate algorithm returns the iterator to the new
    ///           location of the element pointed by first,equal to first +
    ///           (last - new_first).
    ///
    template <typename FwdIter>
    FwdIter rotate(FwdIter first, FwdIter new_first, FwdIter last);

    /// Performs a left rotation on a range of elements. Specifically,
    /// \a rotate swaps the elements in the range [first, last) in such a way
    /// that the element new_first becomes the first element of the new range
    /// and new_first - 1 becomes the last element.
    ///
    /// \note   Complexity: Linear in the distance between \a first and \a last.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter     The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param new_first    Refers to the element that should appear at the
    ///                     beginning of the rotated range.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    ///
    /// The assignments in the parallel \a rotate algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a rotate algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \note The type of dereferenced \a FwdIter must meet the requirements
    ///       of \a MoveAssignable and \a MoveConstructible.
    ///
    /// \returns  The \a rotate algorithm returns a \a pika::future<FwdIter>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or \a parallel_task_policy and
    ///           returns \a FwdIter otherwise.
    ///           The \a rotate algorithm returns the iterator equal to
    ///           first + (last - new_first).
    ///
    template <typename ExPolicy, typename FwdIter>
    typename pika::parallel::detail::algorithm_result<ExPolicy, FwdIter>::type
    rotate(ExPolicy&& policy, FwdIter first, FwdIter new_first, FwdIter last);

    /// Copies the elements from the range [first, last), to another range
    /// beginning at \a dest_first in such a way, that the element
    /// \a new_first becomes the first element of the new range and
    /// \a new_first - 1 becomes the last element.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam FwdIter     The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of
    ///                     an forward iterator.
    /// \tparam OutIter     The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of
    ///                     an output iterator.
    ///
    /// \param first        Refers to the beginning of the sequence of
    ///                     elements the algorithm will be applied to.
    /// \param new_first    Refers to the element that should appear at the
    ///                     beginning of the rotated range.
    /// \param last         Refers to the end of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param dest_first   Refers to the begin of the destination range.
    ///
    /// The assignments in the parallel \a rotate_copy algorithm
    /// execute in sequential order in the calling thread.
    ///
    /// \returns  The \a rotate_copy algorithm returns a output iterator,
    ///           The \a rotate_copy algorithm returns the output iterator
    ///           to the element past the last element copied.
    ///
    template <typename FwdIter, typename OutIter>
    OutIter rotate_copy(
        FwdIter first, FwdIter new_first, FwdIter last, OutIter dest_first);

    /// Copies the elements from the range [first, last), to another range
    /// beginning at \a dest_first in such a way, that the element
    /// \a new_first becomes the first element of the new range and
    /// \a new_first - 1 becomes the last element.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use
    ///                     (deduced). It describes the manner in which the
    ///                     execution of the algorithm may be parallelized
    ///                     and the manner in which it executes the
    ///                     assignments.
    /// \tparam FwdIter1    The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of
    ///                     an forward iterator.
    /// \tparam FwdIter2    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of
    ///                     an forward iterator.
    ///
    /// \param policy       The execution policy to use for the scheduling
    ///                     of the iterations.
    /// \param first        Refers to the beginning of the sequence of
    ///                     elements the algorithm will be applied to.
    /// \param new_first    Refers to the element that should appear at the
    ///                     beginning of the rotated range.
    /// \param last         Refers to the end of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param dest_first   Refers to the begin of the destination range.
    ///
    /// The assignments in the parallel \a rotate_copy algorithm
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a rotate_copy algorithm
    /// execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a rotate_copy algorithm returns a
    ///           \a pika::future<FwdIter2>
    ///           if the execution policy is of type
    ///           \a parallel_task_policy and
    ///           returns FwdIter2
    ///           otherwise.
    ///           The \a rotate_copy algorithm returns the output iterator
    ///           to the element past the last element copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename FwdIter2>
    typename pika::parallel::detail::algorithm_result<ExPolicy, FwdIter2>::type
    rotate_copy(ExPolicy&& policy, FwdIter1 first, FwdIter1 new_first,
        FwdIter1 last, FwdIter2 dest_first);

}    // namespace pika

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/async/dataflow.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/execution/traits/is_execution_policy.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/modules/execution.hpp>
#include <pika/pack_traversal/unwrap.hpp>

#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/algorithms/copy.hpp>
#include <pika/parallel/algorithms/detail/dispatch.hpp>
#include <pika/parallel/algorithms/detail/rotate.hpp>
#include <pika/parallel/algorithms/reverse.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>
#include <pika/parallel/util/result_types.hpp>
#include <pika/parallel/util/transfer.hpp>

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>

namespace pika::parallel::detail {
    ///////////////////////////////////////////////////////////////////////////
    // rotate
    /// \cond NOINTERNAL
    template <typename ExPolicy, typename FwdIter, typename Sent>
    pika::future<in_out_result<FwdIter, Sent>>
    rotate_helper(ExPolicy policy, FwdIter first, FwdIter new_first, Sent last)
    {
        using non_seq = std::false_type;

        auto p = pika::execution::parallel_task_policy()
                     .on(policy.executor())
                     .with(policy.parameters());

        reverse<FwdIter> r;
        return dataflow(
            [=](pika::future<FwdIter>&& f1, pika::future<FwdIter>&& f2) mutable
            -> pika::future<in_out_result<FwdIter, Sent>> {
                // propagate exceptions
                f1.get();
                f2.get();

                pika::future<FwdIter> f = r.call2(p, non_seq(), first, last);
                return f.then([=](pika::future<FwdIter>&& f) mutable
                    -> in_out_result<FwdIter, Sent> {
                    f.get();    // propagate exceptions
                    std::advance(first, (distance) (new_first, last));
                    return in_out_result<FwdIter, Sent>{first, last};
                });
            },
            r.call2(p, non_seq(), first, new_first),
            r.call2(p, non_seq(), new_first, last));
    }

    template <typename IterPair>
    struct rotate : public algorithm<rotate<IterPair>, IterPair>
    {
        rotate()
          : rotate::algorithm("rotate")
        {
        }

        template <typename ExPolicy, typename InIter, typename Sent>
        static IterPair
        sequential(ExPolicy, InIter first, InIter new_first, Sent last)
        {
            return sequential_rotate(first, new_first, last);
        }

        template <typename ExPolicy, typename FwdIter, typename Sent>
        static typename algorithm_result<ExPolicy, IterPair>::type
        parallel(ExPolicy&& policy, FwdIter first, FwdIter new_first, Sent last)
        {
            return algorithm_result<ExPolicy, IterPair>::get(rotate_helper(
                PIKA_FORWARD(ExPolicy, policy), first, new_first, last));
        }
    };
    /// \endcond

    ///////////////////////////////////////////////////////////////////////////
    // rotate_copy
    /// \cond NOINTERNAL

    // sequential rotate_copy
    template <typename InIter, typename Sent, typename OutIter>
    inline in_out_result<InIter, OutIter> sequential_rotate_copy(
        InIter first, InIter new_first, Sent last, OutIter dest_first)
    {
        in_out_result<InIter, OutIter> p1 =
            (copy) (new_first, last, dest_first);
        in_out_result<InIter, OutIter> p2 =
            (copy) (first, new_first, PIKA_MOVE(p1.out));
        return in_out_result<InIter, OutIter>{
            PIKA_MOVE(p1.in), PIKA_MOVE(p2.out)};
    }

    template <typename ExPolicy, typename FwdIter1, typename Sent,
        typename FwdIter2>
    pika::future<in_out_result<FwdIter1, FwdIter2>>
    rotate_copy_helper(ExPolicy policy, FwdIter1 first, FwdIter1 new_first,
        Sent last, FwdIter2 dest_first)
    {
        using non_seq = std::false_type;

        auto p = pika::execution::parallel_task_policy()
                     .on(policy.executor())
                     .with(policy.parameters());

        using copy_return_type = in_out_result<FwdIter1, FwdIter2>;

        pika::future<copy_return_type> f = copy_algo<copy_return_type>().call2(
            p, non_seq(), new_first, last, dest_first);

        return f.then([=](pika::future<copy_return_type>&& result)
                          -> pika::future<copy_return_type> {
            copy_return_type p1 = result.get();
            return copy_algo<copy_return_type>().call2(
                p, non_seq(), first, new_first, p1.out);
        });
    }

    template <typename IterPair>
    struct rotate_copy : public algorithm<rotate_copy<IterPair>, IterPair>
    {
        rotate_copy()
          : rotate_copy::algorithm("rotate_copy")
        {
        }

        template <typename ExPolicy, typename InIter, typename Sent,
            typename OutIter>
        static in_out_result<InIter, OutIter> sequential(ExPolicy, InIter first,
            InIter new_first, Sent last, OutIter dest_first)
        {
            return sequential_rotate_copy(first, new_first, last, dest_first);
        }

        template <typename ExPolicy, typename FwdIter1, typename Sent,
            typename FwdIter2>
        static typename algorithm_result<ExPolicy,
            in_out_result<FwdIter1, FwdIter2>>::type
        parallel(ExPolicy&& policy, FwdIter1 first, FwdIter1 new_first,
            Sent last, FwdIter2 dest_first)
        {
            return algorithm_result<ExPolicy, IterPair>::get(
                rotate_copy_helper(PIKA_FORWARD(ExPolicy, policy), first,
                    new_first, last, dest_first));
        }
    };
    /// \endcond
}    // namespace pika::parallel::detail

// create new APIs, tag_fallback_invoke overloads.
namespace pika {
    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::rotate
    inline constexpr struct rotate_t final
      : pika::detail::tag_parallel_algorithm<rotate_t>
    {
        // clang-format off
        template <typename FwdIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator_v<FwdIter>
            )>
        // clang-format on
        friend FwdIter tag_fallback_invoke(
            pika::rotate_t, FwdIter first, FwdIter new_first, FwdIter last)
        {
            static_assert(pika::traits::is_forward_iterator_v<FwdIter>,
                "Requires at least forward iterator.");

            return parallel::detail::get_second_element(
                pika::parallel::detail::rotate<
                    pika::parallel::detail::in_out_result<FwdIter, FwdIter>>()
                    .call(pika::execution::seq, first, new_first, last));
        }
        // clang-format off
        template <typename ExPolicy, typename FwdIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy_v<ExPolicy> &&
                pika::traits::is_iterator_v<FwdIter>
            )>
        // clang-format on
        friend
            typename parallel::detail::algorithm_result<ExPolicy, FwdIter>::type
            tag_fallback_invoke(pika::rotate_t, ExPolicy&& policy,
                FwdIter first, FwdIter new_first, FwdIter last)
        {
            static_assert((pika::traits::is_forward_iterator_v<FwdIter>),
                "Requires at least forward iterator.");

            using is_seq = std::integral_constant<bool,
                pika::is_sequenced_execution_policy_v<ExPolicy> ||
                    !pika::traits::is_bidirectional_iterator_v<FwdIter>>;

            return parallel::detail::get_second_element(
                pika::parallel::detail::rotate<
                    pika::parallel::detail::in_out_result<FwdIter, FwdIter>>()
                    .call2(PIKA_FORWARD(ExPolicy, policy), is_seq(), first,
                        new_first, last));
        }

    } rotate{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::rotate_copy
    inline constexpr struct rotate_copy_t final
      : pika::detail::tag_parallel_algorithm<rotate_copy_t>
    {
        // clang-format off
        template <typename FwdIter, typename OutIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator_v<FwdIter> &&
                pika::traits::is_iterator_v<OutIter>
            )>
        // clang-format on
        friend OutIter tag_fallback_invoke(pika::rotate_copy_t, FwdIter first,
            FwdIter new_first, FwdIter last, OutIter dest_first)
        {
            static_assert((pika::traits::is_forward_iterator_v<FwdIter>),
                "Requires at least forward iterator.");
            static_assert((pika::traits::is_output_iterator_v<OutIter>),
                "Requires at least output iterator.");

            return parallel::detail::get_second_element(
                pika::parallel::detail::rotate_copy<
                    pika::parallel::detail::in_out_result<FwdIter, OutIter>>()
                    .call(pika::execution::seq, first, new_first, last,
                        dest_first));
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator_v<FwdIter1> &&
                pika::is_execution_policy_v<ExPolicy> &&
                pika::traits::is_iterator_v<FwdIter2>
            )>
        // clang-format on
        friend typename parallel::detail::algorithm_result<ExPolicy,
            FwdIter2>::type
        tag_fallback_invoke(pika::rotate_copy_t, ExPolicy&& policy,
            FwdIter1 first, FwdIter1 new_first, FwdIter1 last,
            FwdIter2 dest_first)
        {
            static_assert((pika::traits::is_forward_iterator_v<FwdIter1>),
                "Requires at least forward iterator.");
            static_assert((pika::traits::is_forward_iterator_v<FwdIter2>),
                "Requires at least forward iterator.");

            using is_seq = std::integral_constant<bool,
                pika::is_sequenced_execution_policy_v<ExPolicy> ||
                    !pika::traits::is_forward_iterator_v<FwdIter1>>;

            return parallel::detail::get_second_element(
                pika::parallel::detail::rotate_copy<
                    pika::parallel::detail::in_out_result<FwdIter1, FwdIter2>>()
                    .call2(PIKA_FORWARD(ExPolicy, policy), is_seq(), first,
                        new_first, last, dest_first));
        }
    } rotate_copy{};
}    // namespace pika

#endif    // DOXYGEN
