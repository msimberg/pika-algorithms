//  Copyright (c) 2020 ETH Zurich
//  Copyright (c) 2014 Grant Mercer
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/adjacent_find.hpp

#pragma once

#if defined(DOXYGEN)

namespace pika {
    /// Searches the range [first, last) for two consecutive identical elements.
    ///
    /// \note   Complexity: Exactly the smaller of (result - first) + 1 and
    ///                     (last - first) - 1 application of the predicate
    ///                     where \a result is the value returned
    ///
    /// \tparam FwdIter     The type of the source iterators used for the
    ///                     range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Pred        The type of an optional function/function object to use.
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     of the range the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements of
    ///                     the range the algorithm will be applied to.
    /// \param pred         The binary predicate which returns \a true
    ///                     if the elements should be treated as equal. The
    ///                     signature should be equivalent to the following:
    ///                     \code
    ///                     bool pred(const Type1 &a, const Type1 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const &, but
    ///                     the function must not modify the objects passed to
    ///                     it. The types \a Type1 must be such
    ///                     that objects of type \a FwdIter
    ///                     can be dereferenced and then implicitly converted
    ///                     to \a Type1 .
    ///
    /// \returns  The \a adjacent_find algorithm returns an iterator to the
    ///           first of the identical elements. If no such elements are
    ///           found, \a last is returned.
    template <typename FwdIter, typename Pred = detail::equal_to>
    FwdIter adjacent_find(FwdIter first, FwdIter last, Pred&& pred = Pred());

    /// Searches the range [first, last) for two consecutive identical elements.
    /// This version uses the given binary predicate pred
    ///
    /// \note   Complexity: Exactly the smaller of (result - first) + 1 and
    ///                     (last - first) - 1 application of the predicate
    ///                     where \a result is the value returned
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter     The type of the source iterators used for the
    ///                     range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Pred        The type of an optional function/function object to use.
    ///                     Unlike its sequential form, the parallel
    ///                     overload of \a adjacent_find requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible. This defaults
    ///                     to std::equal_to<>
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     of the range the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements of
    ///                     the range the algorithm will be applied to.
    /// \param pred         The binary predicate which returns \a true
    ///                     if the elements should be treated as equal. The
    ///                     signature should be equivalent to the following:
    ///                     \code
    ///                     bool pred(const Type1 &a, const Type1 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const &, but
    ///                     the function must not modify the objects passed to
    ///                     it. The types \a Type1 must be such
    ///                     that objects of type \a FwdIter
    ///                     can be dereferenced and then implicitly converted
    ///                     to \a Type1 .
    ///
    /// The comparison operations in the parallel \a adjacent_find invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The comparison operations in the parallel \a adjacent_find invoked
    /// with an execution policy object of type \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an
    /// unordered fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a adjacent_find algorithm returns a \a pika::future<InIter>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a InIter otherwise.
    ///           The \a adjacent_find algorithm returns an iterator to the
    ///           first of the identical elements. If no such elements are
    ///           found, \a last is returned.
    ///
    ///           This overload of \a adjacent_find is available if the user
    ///           decides to provide their algorithm their own binary
    ///           predicate \a pred.
    ///
    template <typename ExPolicy, typename FwdIter,
        typename Pred = detail::equal_to>
    typename std::enable_if<pika::is_execution_policy<ExPolicy>::value,
        typename algorithm_result<ExPolicy, FwdIter>::type>::type
    adjacent_find(
        ExPolicy&& policy, FwdIter first, FwdIter last, Pred&& pred = Pred());
}    // namespace pika
#else

#include <pika/config.hpp>
#include <pika/executors/execution_policy.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/parallel/algorithms/adjacent_find.hpp>
#include <pika/parallel/algorithms/detail/dispatch.hpp>
#include <pika/parallel/algorithms/detail/predicates.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>
#include <pika/parallel/util/invoke_projected.hpp>
#include <pika/parallel/util/loop.hpp>
#include <pika/parallel/util/partitioner.hpp>
#include <pika/parallel/util/projection_identity.hpp>
#include <pika/parallel/util/zip_iterator.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

namespace pika::parallel::detail {
    ///////////////////////////////////////////////////////////////////////////
    // adjacent_find
    /// \cond NOINTERNAL
    template <typename Iter, typename Sent>
    struct adjacent_find : public algorithm<adjacent_find<Iter, Sent>, Iter>
    {
        adjacent_find()
          : adjacent_find::algorithm("adjacent_find")
        {
        }

        template <typename ExPolicy, typename InIter, typename Sent_,
            typename Pred, typename Proj>
        static InIter
        sequential(ExPolicy, InIter first, Sent_ last, Pred&& pred, Proj&& proj)
        {
            return std::adjacent_find(first, last,
                invoke_projected<Pred, Proj>(
                    PIKA_FORWARD(Pred, pred), PIKA_FORWARD(Proj, proj)));
        }

        template <typename ExPolicy, typename FwdIter, typename Sent_,
            typename Pred, typename Proj>
        static typename algorithm_result<ExPolicy, FwdIter>::type
        parallel(ExPolicy&& policy, FwdIter first, Sent_ last, Pred&& pred,
            Proj&& proj)
        {
            using zip_iterator = pika::util::zip_iterator<FwdIter, FwdIter>;
            using reference = typename zip_iterator::reference;
            using difference_type =
                typename std::iterator_traits<FwdIter>::difference_type;

            if (first == last)
            {
                return algorithm_result<ExPolicy, FwdIter>::get(
                    PIKA_MOVE(last));
            }

            FwdIter next = first;
            ++next;
            difference_type count = std::distance(first, last);
            util::cancellation_token<difference_type> tok(count);

            invoke_projected<Pred, Proj> pred_projected{
                PIKA_FORWARD(Pred, pred), PIKA_FORWARD(Proj, proj)};

            auto f1 = [pred_projected = PIKA_MOVE(pred_projected), tok](
                          zip_iterator it, std::size_t part_size,
                          std::size_t base_idx) mutable {
                loop_idx_n<std::decay_t<ExPolicy>>(base_idx, it, part_size, tok,
                    [&pred_projected, &tok](reference t, std::size_t i) {
                        using std::get;
                        if (pred_projected(get<0>(t), get<1>(t)))
                            tok.cancel(i);
                    });
            };

            auto f2 =
                [tok, count, first, last](
                    std::vector<pika::future<void>>&& data) mutable -> FwdIter {
                // make sure iterators embedded in function object that is
                // attached to futures are invalidated
                data.clear();
                difference_type adj_find_res = tok.get_data();
                if (adj_find_res != count)
                    std::advance(first, adj_find_res);
                else
                    first = last;

                return PIKA_MOVE(first);
            };

            return partitioner<ExPolicy, FwdIter, void>::call_with_index(
                PIKA_FORWARD(ExPolicy, policy),
                pika::util::make_zip_iterator(first, next), count - 1, 1,
                PIKA_MOVE(f1), PIKA_MOVE(f2));
        }
    };
    /// \endcond
}    // namespace pika::parallel::detail

namespace pika {
    inline constexpr struct adjacent_find_t final
      : pika::detail::tag_parallel_algorithm<adjacent_find_t>
    {
    private:
        // clang-format off
        template <typename InIter,
            typename Pred = pika::parallel::detail::equal_to,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_input_iterator<InIter>::value
            )>
        // clang-format on
        friend InIter tag_fallback_invoke(pika::adjacent_find_t, InIter first,
            InIter last, Pred&& pred = Pred())
        {
            static_assert((pika::traits::is_input_iterator<InIter>::value),
                "Requires at least input iterator.");

            return parallel::detail::adjacent_find<InIter, InIter>().call(
                pika::execution::seq, first, last, PIKA_FORWARD(Pred, pred),
                pika::parallel::detail::projection_identity{});
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter,
            typename Pred = pika::parallel::detail::equal_to,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_forward_iterator<FwdIter>::value
            )>
        // clang-format on
        friend
            typename parallel::detail::algorithm_result<ExPolicy, FwdIter>::type
            tag_fallback_invoke(pika::adjacent_find_t, ExPolicy&& policy,
                FwdIter first, FwdIter last, Pred&& pred = Pred())
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter>::value),
                "Requires at least a forward iterator");

            return parallel::detail::adjacent_find<FwdIter, FwdIter>().call(
                PIKA_FORWARD(ExPolicy, policy), first, last,
                PIKA_FORWARD(Pred, pred),
                pika::parallel::detail::projection_identity{});
        }
    } adjacent_find{};
}    // namespace pika

#endif
